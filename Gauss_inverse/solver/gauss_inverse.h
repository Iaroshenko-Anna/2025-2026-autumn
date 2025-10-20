#ifndef GAUSS_INVERSE_H
#define GAUSS_INVERSE_H

/*Вычисление обратной матрицы методом Гаусса
 n - размер матрицы, A - исходная матрица, A_inv - обратная матрица*/
bool gauss_inverse(int n, const double* A, double* A_inv);

/*Вычисление нормы невязки для обратной матрицы
 Норма невязки = максимальный элемент |A*A_inv - E|*/
double compute_residual_norm(int n, const double* A, const double* A_inv);

#endif