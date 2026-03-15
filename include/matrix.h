#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include "error_common.h"

/*!
 * @file matrix.h
 * @brief Moduł do obsługi macierzy.
 */

/// 
/// Struktura macierzy. Dane przechowywane w data są w formacie kolumnowym
/// 
typedef struct matrix_st
{
    uint64_t rows;
    uint64_t columns;
    float *data;
} Matrix;


///
///Inicjalizuje macierz, zawsze używać przy tworzeniu nowej macierzy.
///
ErrorEnum matrix_init(uint64_t rows, uint64_t columns, float *data,Matrix **m1);

///
///Jedna funkcja do zwalniania pamięci zajmowanej przez macierz.
///
void matrix_delete(Matrix **m1);

///
///Pobieranie wartości w macierzy na 'i' wierszu i w 'j' kolumnie.
///
ErrorEnum matrix_get_value(uint64_t i, uint64_t j,const Matrix *m1,float *vlue);

///
//Zmienianie wartości w macierzy na 'i' wierszu i w 'j' kolumnie.
///
ErrorEnum matrix_set_value(uint64_t i, uint64_t j,Matrix *m1,float value);

#endif //MATRIX_H