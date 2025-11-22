#ifndef PRINT_H
#define PRINT_H

/* Вывод матрицы на экран
 rows - количество строк, cols - количество столбцов
 A - матрица для вывода, m - максимальное количество выводимых элементов*/
void print_matrix(int rows, int cols, const double* A, int m);

/* Вывод вектора на экран  
 n - размер вектора, v - вектор для вывода, m - максимальное количество выводимых элементов*/
void print_vector(int n, const double* v, int m);

#endif