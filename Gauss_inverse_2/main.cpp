#include <iostream>
#include <cstdlib> 
#include<stdio.h>   
#include <ctime>      
#include "init/matrix.h"     
#include "solver/gauss_inverse.h"  
#include "print/print.h"    

int main(int argc, char* argv[]) {
    
    if (argc < 4 || argc > 6) {
        std::cerr << "Usage: " << argv[0] << " n m k [filename]\n";
        std::cerr << "  n - размер матрицы\n";
        std::cerr << "  m - количество выводимых элементов\n"; 
        std::cerr << "  k - формула (0 - из файла, 1-4 - по формуле)\n";
        std::cerr << "  filename - имя файла (только когда k = 0)\n";
		std::cerr << "  num_threads - количество потоков (необязательно, по умолчанию 4)\n";
        return 1;  // Выход с ошибкой
    }

    int n = std::atoi(argv[1]);  
    int m = std::atoi(argv[2]);  
    int k = std::atoi(argv[3]); 
    int num_threads = 4;
	
    double* A = new double[n * n];      // Исходная матрица A 
    double* A_inv = new double[n * n];  // Обратная матрица A^(-1)

    bool flag = false;  
	
    if (k == 0) {
        // Случай k = 0: читаем матрицу из файла
        if (argc < 5) {
            std::cerr << "Filename required when k = 0\n";
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
		// Если есть 6-й аргумент - это количество потоков
        if (argc == 6) {
            num_threads = std::atoi(argv[5]);
        }
    } else if (k >= 1 && k <= 4) {
         if (argc >= 5) {
            num_threads = std::atoi(argv[4]);
        }
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
	
	if (num_threads <= 0) {
        std::cerr << "Number of threads must be positive\n";
        delete[] A; delete[] A_inv;
        return 1;
    }

    std::cout << "Using " << num_threads << " threads\n";
    std::cout << "Matrix A:\n";
    print_matrix(n, n, A, m);  // Выводим матрицу A (не более m элементов)
    
    // Вычисляем обратную матрицу и замеряем время
    clock_t start = clock();      
    flag = gauss_inverse_parallel(n, A, A_inv, num_threads); 
    clock_t end = clock(); 
    // Проверяем успешность вычисления обратной матрицы
    if (!flag) {
        std::cerr << "Matrix inversion failed\n";
    } else {
        std::cout << "Inverse matrix A^(-1):\n";
        print_matrix(n, n, A_inv, m);  
        // Вычисляем и выводим норму невязки
        double res_norm = compute_residual_norm(n, A, A_inv, num_threads);
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