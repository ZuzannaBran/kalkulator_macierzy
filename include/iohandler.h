#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "matrix.h"
#include "error_common.h"
/*!
 * @file iohandler.h
 * @brief Moduł do obsługi wczytywania i zapisywania plików binarynch i zapisywania tekstowych.
 */

/*!
 * Zapisuje macierz do pliku binarnego.
 *
 * Do pliku zapisywana jest liczba wierszy (uint64_t),
 * a następnie surowe dane macierzy typu float.
 * Kolejność danych w pliku jest taka sama jak w pamięci.
 */
ErrorEnum saveMatrix(Matrix *m, const char *filename);

/*!
 * Wczytuje macierz z pliku binarnego.
 *
 * Funkcja alokuje pamięć na dane macierzy (malloc),
 * odczytuje liczbę wierszy oraz dane typu float
 * i inicjalizuje nową strukturę Matrix.
 */
ErrorEnum loadMatrix(Matrix **m, const char *filename);

/*!
 * Zapisuje macierz do pliku tekstowego.
 *
 * Macierz zapisywana jest w postaci czytelnej dla człowieka:
 * każdy wiersz w osobnej linii, a elementy oddzielone spacjami.
 * Format przechowywania danych w pamięci nie wpływa na wynik zapisu.
 */
ErrorEnum saveMatrixTxt(Matrix *m, const char *filename);

#endif // IOHANDLER_H
