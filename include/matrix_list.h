#ifndef MATRIX_LIST_H
#define MATRIX_LIST_H
#include <stdint.h>
#include "matrix.h"
#include "error_common.h"

#define MATRIX_NAME_SIZE 64
#define INITIAL_MATRIX_LIST_SIZE 2
#define RESIZE_MULTIPLIER 2
/*!
 * @file matrix_list.h
 * @brief Moduł do obsługi list par nazwa-macierz.
 */

///
///Stuktura stworzona do przechowywania pary macierz-nazwa. Ustawia rozmiar na INITIAL_MATRIX_LIST_SIZE
///
typedef struct element_matrix
{
    char name[MATRIX_NAME_SIZE];
    Matrix *matrix;
} ElementMatrix;

///
///Lista par macierz-nazwa z informacją o jej rozmiarze
///
typedef struct matrix_list
{
    uint64_t size;
    ElementMatrix **data;
} MatrixList;

///
///Inicjalizacja listy par macierz-nazwa
///
ErrorEnum matrix_list_init(MatrixList **new_m_list);
///
///Zwolnienie pamięci listy par macierz-nazwa jak i matryc znajudjących się w niej
///
ErrorEnum matrix_list_delete(MatrixList **m_list);

/*!
 *Znalezienie matrycy o nazwie 'name' i zwrócenie jej do '*returned_m', jeżeli nie uda znaleść się nazwy jest zwracane FAILURE.
 *Jeżeli wartość returned_m jest równa NULL, funkcja nadal próbuje znaleść nazwe, ale nie zwraca macierzy
 */
ErrorEnum matrix_list_get(const MatrixList *m_list, char *name, Matrix **returned_m);
///
///Dodanie pary macierz-nazwa do list.
///
ErrorEnum matrix_list_add(MatrixList *m_list,Matrix *m1,char *name);
///
///Usunięcie elementu o nazwie 'name' i zwolnienie pamięci
///
ErrorEnum matrix_list_remove(MatrixList *m_list,char *name);
///
///Zwrócenie posortowanej listy nazw macierz do *list_of_names, ilości elementów do *store_amount.
//
ErrorEnum matrix_list_names_sorted(const MatrixList *m_list,char*** list_of_names, uint64_t *store_amount);
///
///Przemnożenie aktualnego rozmiaru macierzy o RESIZE_MULTIPLIER i odpowiednia alokacja pamięci
///
ErrorEnum matrix_list_resize(MatrixList *m_list);

#endif
