#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdio.h>
#include "matrix_list.h"
#include "error_common.h"

/*!
 * @file script.h
 * @brief Moduł do przetwarzania i wykonywania poleceń z pliku i terminala.
 */


///
/// Wczytuje i wykonuje polecenia z pliku tekstowego.
/// Każda linia jest traktowana jako jedno polecenie.
/// Puste linie są ignorowane.
/// Polecenie "Exit" kończy wczytywanie skrypu.
/// SUCCESS jeśli plik został wczytany i wykonany pomyślnie,
/// FILE_OPENING_ERROR jeśli nie można otworzyć pliku,
/// FILE_READING_ERROR jeśli błąd podczas czytania,
/// NULL_POINTER_ERROR jeśli któryś z parametrów to NULL
///
ErrorEnum run_script_from_file(MatrixList *matrix_list, const char *filename);

///
/// Usuwa końcowe i początkowe białe znaki, modyfikuje łańcuch.
///
void trim_blank_char(char *s);

///
/// Sprawdza, czy linia zawiera tylko białe znaki.
/// Zwraca 1 jeśli linia jest pusta, a 0 jeśli zawiera znaki.
///
int is_blank_line(const char *s);

///
/// Główna funkcja do przetworzenia jednej linii polecenia.
/// zwraca SUCCESS jeśli operacja się powiodła, CMD_BAD_ARGS, lub rezultat cmd_assignment.
///
ErrorEnum run_script_line(MatrixList *matrix_list, const char *line);

///
/// Wyświetla liste wszystkich macierzy w bazie danych, posortowaną alfabetycznie.
/// Dla każdej macierzy pokazuje wymiary i zajęty rozmiar w bajtach.
/// Zwraca SUCCESS jeśli operacja się powiodła, EMPTY jeśli baza danych jest pusta lub rezultat z matrix_list_get(), MATRIX_NOT_FOUND jeśli miacierz nie istnieje.
///
ErrorEnum cmd_list(MatrixList *matrix_list);

///
/// Wyświetla całą macierz na ekranie z wyrazami oraz wartościami elementów.
/// zwraca SUCCESS jeśli macierz istnieje, MATRIX_NOT_FOUND jeśli macierz nie istnieje, lub rezultat z matrix_list_get().
///
ErrorEnum cmd_print(MatrixList *matrix_list, const char *name);

///
/// Obsługuje operacje Load, Save, Txt.
/// Zwraca SUCCESS jeśli operacja się powiodła, MALLOC_ERROR jeśli jest problem z malloc,MATRIX_NOT_FOUND jeśli macierz nie istnieje, CMD_BAD_ARGS jeśli zostały podane złe argumenty, lub rezultat loadMatrix(), matrix_list_add(),matrix_list_get(), saveMatrix(), matrix_list_get(),saveMatrixTxt()
///
ErrorEnum cmd_io(MatrixList *matrix_list, const char *cmd, const char *a, const char *b);

///
/// Obsługuje przypisania z operacjami arytmetycznymi na macierzach.
/// Zwraca SUCCESS jeśli operacja się powiodła, MATRIX_NOT_FOUND jeśli macierz nie istnieje,CMD_BAD_ARGS jeśli są podane złe argumenty,lub resultat matrix_list_get(),matrix_subtract(),matrix_multiply(),matrix_concat_horizontal(),matrix_list_add()
///
ErrorEnum cmd_assignment(MatrixList *matrix_list, const char *left, const char *l, const char *op, const char *r);

#endif
