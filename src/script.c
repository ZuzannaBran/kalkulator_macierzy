// plik zawierający funckje do obsługi main

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/error_common.h"
#include "../include/matrix_list.h"
#include "../include/operations.h"
#include "../include/iohandler.h"
#include "../include/script.h"
#include "../include/matrix.h"

#define LINE_MAX 1024

///
/// sprawdza, czy linia zawiera tylko białe znaki
/// zwraca 1 jeśli linia jest pusta, a 0 jeśli zawiera znaki
///
int is_blank_line(const char *str)
{
    size_t i;
    if (str == NULL)
    {
        return 1;
    }
    for (i = 0; str[i] != '\0'; i++)
    {
        if (!isspace((unsigned char)str[i]))
        {
            return 0;
        }
    }
    return 1;
}

///
/// usuwa końcowe i początkowe białe znaki, modyfikuje łańcuch
///
void trim_blank_char(char *str)
{
    size_t start = 0;
    size_t end;
    size_t i;

    if (str == NULL)
    {
        return;
    }

    while (str[start] != '\0' && isspace((unsigned char)str[start]))
    {
        start++;
    }

    end = strlen(str);
    while (end > start && isspace((unsigned char)str[end - 1]))
    {
        end--;
    }

    if (start > 0)
    {
        for (i = 0; i + start < end; i++)
        {
            str[i] = str[i + start];
        }
        str[i] = '\0';
    }
    else
    {
        str[end] = '\0';
    }
}
///
/// wyświetla liste wszystkich macierzy w bazie danych, posortowaną alfabetycznie
/// dla każdej macierzy pokazuje wymiary i zajęty rozmiar w bajtach
/// zwraca SUCCESS jeśli operacja się powiodła, EMPTY jeśli baza danych jest pusta lub rezultat z matrix_list_get(), MATRIX_NOT_FOUND jeśli miacierz nie istnieje
///
ErrorEnum cmd_list(MatrixList *matrix_list)
{
    char **names = NULL;
    uint64_t count = 0;
    ErrorEnum result = matrix_list_names_sorted(matrix_list, &names, &count);
    if (result != SUCCESS)
        return result;

    if (count == 0)
    {
        free(names);
        return EMPTY;
    }

    for (uint64_t i = 0; i < count; i++)
    {
        Matrix *m = NULL;
        result = matrix_list_get(matrix_list, names[i], &m);
        if (result == SUCCESS && m)
        {
            uint64_t bytes = m->rows * m->columns * sizeof(float);
            printf("%s: %llu x %llu (%llu bajtów)\n",
                   names[i],
                   (unsigned long long)m->rows,
                   (unsigned long long)m->columns,
                   (unsigned long long)bytes);
        }
        else
        {
            if (result == SUCCESS)
                result = MATRIX_NOT_FOUND;

            free(names);

            return result;
        }
    }

    free(names);
    return SUCCESS;
}

ErrorEnum cmd_print(MatrixList *matrix_list, const char *name)
{
    Matrix *m = NULL;
    ErrorEnum result = matrix_list_get(matrix_list, (char *)name, &m);
    if (result != SUCCESS)
    {
        return result; // zwraca szczegółowy błąd z matrix_list_get
    }
    if (m == NULL)
    {
        return MATRIX_NOT_FOUND;
    }
    printf("%s: %llu x %llu\n", name,
           (unsigned long long)m->rows, (unsigned long long)m->columns);
    for (uint64_t i = 0; i < m->rows; i++)
    {
        for (uint64_t j = 0; j < m->columns; j++)
        {
            float v = 0.0f;
            if (matrix_get_value(i, j, m, &v) == SUCCESS)
                printf("%.6f%s", v, (j + 1 < m->columns ? " " : ""));
            else
                printf("ERR%s", (j + 1 < m->columns ? " " : ""));
        }
        printf("\n");
    }
    return SUCCESS;
}



ErrorEnum cmd_io(MatrixList *matrix_list, const char *cmd, const char *a, const char *b)
{
    ErrorEnum result;
    if (strcmp(cmd, "Load") == 0)
    {
        Matrix *m = malloc(sizeof(Matrix));
        if (!m)
        {
            return MALLOC_ERROR;
        }

        result = loadMatrix(&m, a);
        if (result != SUCCESS)
        {
            free(m);
            return result;
        }

        result = matrix_list_add(matrix_list, m, (char *)b);
        if (result != SUCCESS)
        {
            matrix_delete(&m);
            return result;
        }
        return SUCCESS;
    }
    else if (strcmp(cmd, "Save") == 0)
    {
        Matrix *m = NULL;
        result = matrix_list_get(matrix_list, (char *)a, &m);
        if (result != SUCCESS)
        {
            return result;
        }
        if (!m)
        {
            return MATRIX_NOT_FOUND;
        }

        return saveMatrix(m, b);
    }
    else if (strcmp(cmd, "Txt") == 0)
    {
        Matrix *m = NULL;
        result = matrix_list_get(matrix_list, (char *)a, &m);
        if (result != SUCCESS)
        {
            return result;
        }
        if (!m)
        {
            return MATRIX_NOT_FOUND;
        }

        result = saveMatrixTxt(m, b);
        return result;
    }

    return CMD_BAD_ARGS;
}

ErrorEnum cmd_assignment(MatrixList *matrix_list, const char *left, const char *l, const char *op, const char *r)
{
    Matrix *m1 = NULL, *m2 = NULL, *res = NULL;
    ErrorEnum result = matrix_list_get(matrix_list, (char *)l, &m1);
    if (result != SUCCESS || !m1)
    {
        if (result == SUCCESS)
            result = MATRIX_NOT_FOUND;
        return result;
    }

    result = matrix_list_get(matrix_list, (char *)r, &m2);
    if (result != SUCCESS || !m2)
    {
        if (result == SUCCESS)
            result = MATRIX_NOT_FOUND;
        return result;
    }

    if (strcmp(op, "-") == 0)
    {
        result = matrix_subtract(m1, m2, &res);
    }
    else if (strcmp(op, "*") == 0)
    {
        result = matrix_multiply(m1, m2, &res);
    }
    else if (strcmp(op, "#") == 0)
    {
        result = matrix_concat_horizontal(m1, m2, &res);
    }
    else
    {
        return CMD_BAD_ARGS;
    }
    if (result != SUCCESS)
    {
        return result;
    }

    result = matrix_list_add(matrix_list, res, (char *)left);
    if (result != SUCCESS)
    {
        matrix_delete(&res);
        return result;
    }

    return SUCCESS;
}

ErrorEnum run_script_line(MatrixList *matrix_list, const char *line_in)
{
    assert(matrix_list != NULL);
    assert(line_in != NULL);

    char line[LINE_MAX];
    strncpy(line, line_in, sizeof(line));
    line[sizeof(line) - 1] = '\0';
    trim_blank_char(line);

    if (is_blank_line(line))
        return SUCCESS;

    // List
    if (strcmp(line, "List") == 0)
        return cmd_list(matrix_list);

    char buf[LINE_MAX];
    strncpy(buf, line, sizeof(buf));
    buf[sizeof(buf) - 1] = '\0';
    char *tok1 = strtok(buf, " \t");
    if (!tok1)
        return SUCCESS;

    // print
    if (strcmp(tok1, "Print") == 0)
    {
        char *name = strtok(NULL, " \t");
        char *extra = strtok(NULL, " \t");
        if (name && !extra)
        {
            return cmd_print(matrix_list, name);
        }
        return CMD_BAD_ARGS;
    }
    // delate
    if (strcmp(tok1, "Delete") == 0)
    {
        char *name = strtok(NULL, " \t");
        char *extra = strtok(NULL, " \t");
        if (name && !extra)
            return matrix_list_remove(matrix_list, name);
        return CMD_BAD_ARGS;
    }
    // run
    if (strcmp(tok1, "Run") == 0)
    {
        char *fname = strtok(NULL, " \t");
        char *extra = strtok(NULL, " \t");
        if (fname && !extra)
            return run_script_from_file(matrix_list, fname);
        return CMD_BAD_ARGS;
    }
    // load/save/txt
    if (strcmp(tok1, "Load") == 0 || strcmp(tok1, "Save") == 0 || strcmp(tok1, "Txt") == 0)
    {

        const char *after_cmd = line + (tok1 - buf + strlen(tok1));
        while (*after_cmd && isspace((unsigned char)*after_cmd))
            after_cmd++;

        char *gt = strchr(after_cmd, '>');
        if (!gt)
        {
            return CMD_BAD_ARGS;
        }

        char left[LINE_MAX], right[LINE_MAX];
        size_t left_len = (size_t)(gt - after_cmd);
        if (left_len >= sizeof(left))
        {
            return CMD_BAD_ARGS;
        }
        strncpy(left, after_cmd, left_len);
        left[left_len] = '\0';
        strncpy(right, gt + 1, sizeof(right));
        right[sizeof(right) - 1] = '\0';

        trim_blank_char(left);
        trim_blank_char(right);

        if (left[0] == '\0' || right[0] == '\0')
        {
            return CMD_BAD_ARGS;
        }

        return cmd_io(matrix_list, tok1, left, right);
    }

    // przypisania z operacjami
    //operacje na macierzach
    char *eq = strchr(line, '=');
    if (eq)
    {
        char left[LINE_MAX], right[LINE_MAX];
        size_t ll = (size_t)(eq - line);
        if (ll >= sizeof(left))
        {
            return CMD_BAD_ARGS;
        }
        strncpy(left, line, ll);
        left[ll] = '\0';
        strncpy(right, eq + 1, sizeof(right));
        right[sizeof(right) - 1] = '\0';
        trim_blank_char(left);
        trim_blank_char(right);

        char *op_pos = NULL;
        char op_char = '\0';

        for (size_t i = 0; right[i] != '\0'; i++)
        {
            if (right[i] == '-' || right[i] == '*' || right[i] == '#')
            {
                op_pos = &right[i];
                op_char = right[i];
                break;
            }
        }

        if (!op_pos)
        {
            return CMD_BAD_ARGS;
        }

        char l[LINE_MAX], r[LINE_MAX], op[2];
        size_t l_len = (size_t)(op_pos - right);
        if (l_len >= sizeof(l))
        {
            return CMD_BAD_ARGS;
        }

        strncpy(l, right, l_len);
        l[l_len] = '\0';
        strncpy(r, op_pos + 1, sizeof(r));
        r[sizeof(r) - 1] = '\0';

        trim_blank_char(l);
        trim_blank_char(r);

        op[0] = op_char;
        op[1] = '\0';

        if (!left[0] || !l[0] || !r[0])
        {
            return CMD_BAD_ARGS;
        }

        return cmd_assignment(matrix_list, left, l, op, r);
    }

    return CMD_BAD_ARGS;
}

ErrorEnum run_script_from_file(MatrixList *matrix_list, const char *filename)
{
    assert(matrix_list != NULL);
    assert(filename != NULL);

    FILE *f = fopen(filename, "r");
    if (!f)
        return FILE_OPENING_ERROR;

    char line[LINE_MAX];
    while (fgets(line, sizeof(line), f) != NULL)
    {
        trim_blank_char(line);
        if (is_blank_line(line))
            continue;
        //Sluży wypisaniu linijek, by mimikować to wypiswanie użytkownika
        puts(line);
        if (strcmp(line, "Exit") == 0)
            break;

        
        print_cmd_error(run_script_line(matrix_list, line));
    }

    if (ferror(f))
    {
        fclose(f);
        return FILE_READING_ERROR;
    }
    fclose(f);
    return SUCCESS;
}
