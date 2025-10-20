#include <iostream>
#include <cstdlib> 
#include<stdio.h>   
#include <ctime>      
#include "init/matrix.h"     // Функции для создания матриц
#include "solver/gauss_inverse.h"   // Функции для решения систем
#include "print/print.h"    // Функции для вывода

int main(int argc, char* argv[]) {
    
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " n m k [filename]\n";
        std::cerr << "  n - размер матрицы\n";
        std::cerr << "  m - количество выводимых элементов\n"; 
        std::cerr << "  k - формула (0 - из файла, 1-4 - по формуле)\n";
        std::cerr << "  filename - имя файла (только когда k = 0)\n";
        return 1;  // Выход с ошибкой
    }

    int n = std::atoi(argv[1]);  
    int m = std::atoi(argv[2]);  
    int k = std::atoi(argv[3]); 
	
    double* A = new double[n * n];      // Исходная матрица A 
    double* A_inv = new double[n * n];  // Обратная матрица A^(-1)

    bool flag = false;  
	
    if (k == 0) {
        // Случай k = 0: читаем матрицу из файла
        if (argc != 5) {
            std::cerr << "Filename required when k = 0\n";
            // Освобождаем память перед выходом
            delete[] A; delete[] A_inv; 
            return 1;
        }
        const char* filename = argv[4];  // Получаем имя файла из аргументов
        flag = read_matrix_from_file(n, A, filename);  // Пытаемся прочитать файл
        if (!flag) {
            std::cerr << "Cannot open or read file: " << filename << "\n";
            delete[] A; delete[] A_inv; 
            return 1;
        }
    } else if (k >= 1 && k <= 4) {
        // Случай k = 1 - 4: создаем матрицу по формуле
        flag = init_matrix_formula(k, n, A);
        if (!flag) {
            std::cerr << "Error initializing matrix with formula " << k << "\n";
            delete[] A; delete[] A_inv; 
            return 1;
        }
    } else {
        // Некорректное значение k
        std::cerr << "Invalid formula k: " << k << "\n";
        delete[] A; delete[] A_inv; 
        return 1;
    }

    std::cout << "Matrix A:\n";
    print_matrix(n, n, A, m);  // Выводим матрицу A (не более m элементов)
    
    // Вычисляем обратную матрицу и замеряем время
    clock_t start = clock();      
    flag = gauss_inverse(n, A, A_inv); 
    clock_t end = clock(); 
    // Проверяем успешность вычисления обратной матрицы
    if (!flag) {
        std::cerr << "Matrix inversion failed\n";
    } else {
        std::cout << "Inverse matrix A^(-1):\n";
        print_matrix(n, n, A_inv, m);  
        // Вычисляем и выводим норму невязки
        double res_norm = compute_residual_norm(n, A, A_inv);
        //std::cout << "Residual norm ||A*A^(-1) - E|| = " << res_norm << "\n";
		printf("Residual norm ||A*A^(-1) - E|| = %e \n", res_norm);
    }

    // Выводим время выполнения
    double time_elapsed = double(end - start) / CLOCKS_PER_SEC;
    //std::cout << "Time elapsed: " << time_elapsed << " sec\n";
	printf("Time %e \n", time_elapsed);
    delete[] A;
    delete[] A_inv;
    
    return 0;
}