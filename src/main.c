/*
Projekt POPRO – Zadanie 4

---Komendy---
Exit
List
Print nazwa
Delete nazwa
Run plik
Load plik > nazwa
Save nazwa > plik
Txt  nazwa > plik
nazwa1 = nazwa2 - nazwa3
nazwa1 = nazwa2 * nazwa3
nazwa1 = nazwa2 # nazwa3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/error_common.h"
#include "../include/matrix_list.h"
#include "../include/operations.h"
#include "../include/iohandler.h"
#include "../include/script.h"

#define LINE_MAX 1024

ErrorEnum run_script_line(MatrixList *matrix_list, const char *line);

// GLOBALNA BAZA DANYCH
static MatrixList *g_matrix_list = NULL;

int main(void)
{
    // Inicjalizacja
    if (matrix_list_init(&g_matrix_list) != SUCCESS)
    {
        fprintf(stderr, "Blad inicjalizacji\n");
        return EXIT_FAILURE;
    }

    char line[LINE_MAX];
    while (1)
    {
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("Nie udalo sie wczytac polecenia, sprobuj ponownie.\n");
            clearerr(stdin);
            continue;
        }
        trim_blank_char(line);

        if (is_blank_line(line))
            continue;

        if (strcmp(line, "Exit") == 0)
            break;

        // Jeden punkt wejścia do parsowania i wykonania polecenia
        print_cmd_error(run_script_line(g_matrix_list, line));
    }

    matrix_list_delete(&g_matrix_list);
    return EXIT_SUCCESS;
}
