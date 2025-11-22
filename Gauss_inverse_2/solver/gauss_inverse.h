#ifndef GAUSS_INVERSE_H
#define GAUSS_INVERSE_H

struct ThreadData {
    int thread_id;          // Номер потока (0, 1, 2, 3 и т.д.)
    int n;                  // Размер матрицы
    int current_row;        // Текущая строка 
    const double* A;        
    const double* A_inv;    
    double* matrix_full;   
    double diag_element;    
    double max_error;       
    int num_threads;
};

bool gauss_inverse_parallel(int n, const double* A, double* A_inv, int num_threads);
double compute_residual_norm(int n, const double* A, const double* A_inv, int num_threads);

//Вспомогательные функции для работы с потоками
void run_threads(int num_threads, void* (*thread_func)(void*), ThreadData* thread_data);
void setup_normalize_threads(ThreadData* thread_data, int n, int current_row, double* matrix_full, double diag, int num_threads);
void setup_eliminate_threads(ThreadData* thread_data, int n, int current_row, double* matrix_full, int num_threads);
void setup_error_threads(ThreadData* thread_data, int n, const double* A, const double* A_inv, int num_threads);

#endif