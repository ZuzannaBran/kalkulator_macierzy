#include <stdio.h>
#include "error_common.h"

// plik z funkcjami do obsługi errorów

void print_cmd_error(int error)
{
    if (error == SUCCESS)
        return;

    printf(ANSI_COLOR_RED"Error: "ANSI_COLOR_RESET);

    switch (error)
    {
    case CMD_BAD_ARGS:
        puts("Niepoprawne argumenty polecenia.");
        break;
    case MALLOC_ERROR:
        puts("Blad alokacji pamieci (malloc).");
        break;
    case OUT_OF_BOUNDS:
        puts("Indeks poza zakresem.");
        break;
    case FILE_OPENING_ERROR:
        puts("Nie udalo sie otworzyc pliku.");
        break;
    case FILE_CLOSING_ERROR:
        puts("Blad podczas zamykania pliku.");
        break;
    case FILE_WRITE_ERROR:
        puts("Blad zapisu do pliku.");
        break;
    case FILE_READING_ERROR:
        puts("Blad odczytu z pliku.");
        break;
    case FILE_FORMAT_ERROR:
        puts("Niepoprawny format pliku.");
        break;
    case FILE_SEEK_ERROR:
        puts("Blad pozycjonowania w pliku (seek).");
        break;
    case MATRIX_FORMAT_ERROR:
        puts("Niepoprawny format macierzy.");
        break;
    case FAILURE:
        puts("Operacja nieudana (ogolny blad).");
        break;
    case EMPTY:
        puts("Baza danych jest pusta.");
        break;
    case MATRIX_NOT_FOUND:
        puts("Wskazana macierz nie istnieje.");
        break;
    case MATRIX_NAME_EXIST:
        puts("Macierz o wskazanej nazwie juz istnieje");
        break;
    case MATRIX_ALREADY_EXISTS:
        puts("Macierz o podanej nazwie już istnieje");
        break;
    case NULL_POINTER_ERROR:
        puts("Brak macierzy lub brak wskaznika na bufor");
        break;
    case CONCAT_ERROR:
        puts("Blad laczenia macierzy");
        break;
    case MULTIPLY_ERROR:
        puts("Nie spelnione warunki mnozenia");
    default:
        puts("nieznany kod bledu.");
        break;
    }
}
