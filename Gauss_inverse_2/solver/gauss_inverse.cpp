#include "gauss_inverse.h"
#include <cmath>
#include <algorithm>
#include <pthread.h>
#include <iostream>

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С ПОТОКАМИ
// ============================================================================

// Функция для запуска и ожидания завершения потоков
void run_threads(int num_threads, void* (*thread_func)(void*), ThreadData* thread_data) {
    pthread_t* threads = new pthread_t[num_threads];
    
    // Создаем потоки
    for (int t = 0; t < num_threads; t++) {
        pthread_create(&threads[t], nullptr, thread_func, &thread_data[t]);
    }
    
    // Ожидаем завершения всех потоков
    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], nullptr);
    }
    
    delete[] threads;
}

// Настройка данных потоков для нормализации строки
void setup_normalize_threads(ThreadData* thread_data, int n, int current_row, 
                            double* matrix_full, double diag, int num_threads) {
    for (int t = 0; t < num_threads; t++) {
        thread_data[t].thread_id = t;
        thread_data[t].n = n;
        thread_data[t].current_row = current_row;
        thread_data[t].matrix_full = matrix_full;
        thread_data[t].diag_element = diag;
        thread_data[t].num_threads = num_threads;
    }
}

// Настройка данных потоков для исключения элементов
void setup_eliminate_threads(ThreadData* thread_data, int n, int current_row, 
                            double* matrix_full, int num_threads) {
    for (int t = 0; t < num_threads; t++) {
        thread_data[t].thread_id = t;
        thread_data[t].n = n;
        thread_data[t].current_row = current_row;
        thread_data[t].matrix_full = matrix_full;
        thread_data[t].num_threads = num_threads;
    }
}

// Настройка данных потоков для вычисления ошибки
void setup_error_threads(ThreadData* thread_data, int n, const double* A, 
                        const double* A_inv, int num_threads) {
    for (int t = 0; t < num_threads; t++) {
        thread_data[t].thread_id = t;
        thread_data[t].n = n;
        thread_data[t].A = A;
        thread_data[t].A_inv = A_inv;
        thread_data[t].max_error = 0.0;
        thread_data[t].num_threads = num_threads;
    }
}

void* normalize_row_thread(void* arg) { //Делит все элементы текущей строки на диагональный элемент
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;
    int i = data->current_row;
    double* matrix_full = data->matrix_full;
    double diag = data->diag_element;
    int num_threads = data->num_threads;
    
    for (int j = data->thread_id; j < 2*n; j += num_threads) {
        matrix_full[i * (2*n) + j] /= diag;
    }
    
    return nullptr;
}

void* eliminate_column_thread(void* arg) { //Зануляет столбец
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;
    int i = data->current_row;
    double* matrix_full = data->matrix_full;
	int num_threads = data->num_threads;
    
    for (int k = data->thread_id; k < n; k += num_threads) {
        if (k != i) {
            double element = matrix_full[k * (2*n) + i];
            for (int j = 0; j < 2*n; j++) {
                matrix_full[k * (2*n) + j] -= element * matrix_full[i * (2*n) + j];
            }
        }
    }
    
    return nullptr;
}

void* compute_error_thread(void* arg) { //вычисление ошибки
    ThreadData* data = (ThreadData*)arg;
    data->max_error = 0.0;
    int num_threads = data->num_threads;
    
    for (int i = data->thread_id; i < data->n; i += num_threads) {
        for (int j = 0; j < data->n; j++) {
            double sum = 0.0;
            for (int k = 0; k < data->n; k++) {
                sum += data->A[i * data->n + k] * data->A_inv[k * data->n + j];
            }
            double expected_value = (i == j) ? 1.0 : 0.0;
            double error = std::abs(sum - expected_value);
          
            if (error > data->max_error) {
                data->max_error = error;
            }
        }
    }
    
    return nullptr;
}

// ============================================================================
// Распараллеленная версия обращения матрицы
// ============================================================================

bool gauss_inverse_parallel(int n, const double* A, double* A_inv, int num_threads) {
    double* matrix_full = new double[n * 2 * n];
    ThreadData* thread_data = new ThreadData[num_threads];
    
    // Заполняем расширенную матрицу [A|E]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix_full[i * (2*n) + j] = A[i * n + j];
        }
        for (int j = 0; j < n; j++) {
            matrix_full[i * (2*n) + (j + n)] = (i == j) ? 1.0 : 0.0;
        }
    }
    
    // Прямой ход метода Гаусса 
    for (int i = 0; i < n; i++) {
        double diag = matrix_full[i * (2*n) + i];
        
        if (std::abs(diag) < 1e-15) {
            delete[] matrix_full;
            delete[] thread_data;
            return false;  
        }
		
        setup_normalize_threads(thread_data, n, i, matrix_full, diag, num_threads);
        run_threads(num_threads, normalize_row_thread, thread_data);
        
        setup_eliminate_threads(thread_data, n, i, matrix_full, num_threads);
        run_threads(num_threads, eliminate_column_thread, thread_data);
    }
    // Извлекаем обратную матрицу
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A_inv[i * n + j] = matrix_full[i * (2*n) + (j + n)];
        }
    }
    
    delete[] matrix_full; 
	delete[] thread_data;	
    return true;  
}

// ============================================================================
// Распараллеленная норма невязки
// ============================================================================

double compute_residual_norm(int n, const double* A, const double* A_inv, int num_threads) {
    ThreadData* thread_data = new ThreadData[num_threads];
    
    setup_error_threads(thread_data, n, A, A_inv, num_threads);
    run_threads(num_threads, compute_error_thread, thread_data);
    
    double global_max_error = 0.0;
    for (int t = 0; t < num_threads; t++) {
        if (thread_data[t].max_error > global_max_error) {
            global_max_error = thread_data[t].max_error;
        }
    }
    
    delete[] thread_data;
    return global_max_error;  
}