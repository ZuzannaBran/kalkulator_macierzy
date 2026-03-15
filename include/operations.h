#ifndef OPERATIONS_H
#define OPERATIONS_H


#include <stdint.h>
#include "matrix.h"
#include "error_common.h"
/*!
 * @file operations.h
 * @brief Moduł operacji na macierzy.
 */

/*!
 * Odejmowanie macierzy a i b, wynik w *result
 * Odejmowanie macierzy: result = a - b.
 * Wymaganie: a i b muszą mieć te same wymiary.
 */
ErrorEnum matrix_subtract(const Matrix *a, const Matrix *b, Matrix **result);

/*!
 * Mnożenie macierzy a i b, wynik w *result
 * Wymaganie: liczba kolumn w a musi być równa liczbie wierszy w b.
 */
ErrorEnum matrix_multiply(const Matrix *a, const Matrix *b, Matrix **result);

/*!
 * Lączenie poziomo macierzy, wynik w *result lewo#prawo.
 * Wymaganie: liczba wierszy w left musi być równa liczbie wierszy w right.
 */
ErrorEnum matrix_concat_horizontal(const Matrix *left, const Matrix *right, Matrix **result);


#endif //OPERATIONS_H
