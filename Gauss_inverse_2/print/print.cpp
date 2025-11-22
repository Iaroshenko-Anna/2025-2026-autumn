#include "print.h"
#include <iostream>
#include <iomanip>

void print_matrix(int rows, int cols, const double* A, int m) {
    int print_rows = (m < rows) ? m : rows;
    int print_cols = (m < cols) ? m : cols;
    
    // Выводим матрицу построчно
    for (int i = 0; i < print_rows; i++) {
        for (int j = 0; j < print_cols; j++) {
            
            std::cout << " " << std::scientific << std::setw(10) << std::setprecision(3) 
                      << A[i * cols + j];
        }
        std::cout << "\n";  
    }
}