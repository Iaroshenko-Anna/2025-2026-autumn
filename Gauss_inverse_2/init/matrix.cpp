#include "matrix.h"
#include <iostream>
#include <fstream>   
#include <cmath>     

// Функция чтения матрицы из txt
bool read_matrix_from_file(int n, double* A, const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        return false;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!(file >> A[i * n + j])) {
                return false;  // Недостаточно данных
            }
        }
    }
    return true; 
}

// Функция создания матрицы по одной из 4 формул
bool init_matrix_formula(int k, int n, double* A) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int row = i + 1;  
            int col = j + 1;  
            switch (k) {
                case 1: 
                    // Формула 1: n - max(i,j) + 1
                    A[i * n + j] = n - std::max(row, col) + 1; 
                    break;
                case 2: 
                    // Формула 2: max(i,j)
                    A[i * n + j] = std::max(row, col); 
                    break;
                case 3: 
                    // Формула 3: |i - j|
                    A[i * n + j] = std::abs(row - col); 
                    break;
                case 4: 
                    // Формула 4: 1/(i + j - 1)
                    A[i * n + j] = 1.0 / (row + col - 1); 
                    break;
                default: 
                    return false;  
            }
        }
    }
    return true;  
}

