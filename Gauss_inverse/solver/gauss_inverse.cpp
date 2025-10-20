#include "gauss_inverse.h"
#include <cmath>
#include <algorithm>

bool gauss_inverse(int n, const double* A, double* A_inv) {
    /* Создаем расширенную матрицу [A | E] размером n x (2n)
       Левая часть - исходная матрица A
       Правая часть n x n - единичная матрица E*/
    double* matrix_full = new double[n * 2 * n];
    
    // Заполняем расширенную матрицу:
    for (int i = 0; i < n; i++) {
        // Копируем исходную матрицу A в левую часть
        for (int j = 0; j < n; j++) {
            matrix_full[i * (2*n) + j] = A[i * n + j];
        }
        // Заполняем правую часть единичной матрицей
        for (int j = 0; j < n; j++) {
            // На диагонали - 1, вне диагонали - 0
            matrix_full[i * (2*n) + (j + n)] = (i == j) ? 1.0 : 0.0;
        }
    }
    
    // Прямой ход метода Гаусса
    for (int i = 0; i < n; i++) {
        // Проверка на вырожденность (диагональный элемент)
        double diag = matrix_full[i * (2*n) + i];
        
        // Если диагональный элемент близок к нулю - матрица вырождена
        if (std::abs(diag) < 1e-15) {
            delete[] matrix_full;  
            return false;  
        }
        
        // Деление всех элементов текущей строки на диагональный элемент
        for (int j = 0; j < 2*n; j++) {
            matrix_full[i * (2*n) + j] /= diag;
        }
        
        // Исключение элементов в столбце i
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double element = matrix_full[k * (2*n) + i];
                for (int j = 0; j < 2*n; j++) {
                    matrix_full[k * (2*n) + j] -= element * matrix_full[i * (2*n) + j];
                }
            }
        }
    }
    
    // После завершения метода Гаусса в правой части расширенной матрицы находится обратная матрица A^(-1)
    // Извлекаем обратную матрицу из правой части расширенной матрицы
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A_inv[i * n + j] = matrix_full[i * (2*n) + (j + n)];
        }
    }
    
    delete[] matrix_full;  
    return true;  
}

// Функция вычисления нормы невязки для обратной матрицы
double compute_residual_norm(int n, const double* A, const double* A_inv) {
    double max_error = 0.0;  
    
    // Вычисляем A * A_inv - E
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            // Умножаем i-ю строку матрицы A на j-й столбец матрицы A_inv
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * A_inv[k * n + j];
            }
            
            double x = (i == j) ? 1.0 : 0.0;
            double error = std::abs(sum - x);  // Отклонение
            
            if (error > max_error) {
                max_error = error;
            }
        }
    }
    
    return max_error;  
}