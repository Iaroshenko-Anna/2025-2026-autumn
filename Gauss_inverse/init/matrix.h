#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>

/* Чтение матрицы из файла
 n - размер матрицы, A - указатель на массив для хранения матрицы
 filename - имя файла с данными*/
bool read_matrix_from_file(int n, double* A, const char* filename);

/* Инициализация матрицы по формуле 1-4
 k - номер формулы, n - размер матрицы, A - указатель на массив*/
bool init_matrix_formula(int k, int n, double* A);

// Создание вектора правой части b
std::vector<double> createVectorB(const std::vector<std::vector<double>>& A, int n);

#endif