#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "error_common.h"
#include "matrix.h"
#include "matrix_list.h"

ErrorEnum matrix_list_init(MatrixList **m_list)
{
    assert(m_list != NULL);

    MatrixList *new_m_list = malloc(sizeof(MatrixList));
    if (new_m_list == NULL)
        return MALLOC_ERROR;

    new_m_list->size = INITIAL_MATRIX_LIST_SIZE;
    new_m_list->data = calloc(sizeof(ElementMatrix *), INITIAL_MATRIX_LIST_SIZE);

    if (new_m_list->data == NULL)
        return MALLOC_ERROR;
    *m_list = new_m_list;
    return SUCCESS;
}
ErrorEnum matrix_list_delete(MatrixList **m_list)
{
    assert(m_list != NULL);
    assert((*m_list) != NULL);

    for (int i = 0; i < (*m_list)->size; i++)
    {
        ElementMatrix *current = (*m_list)->data[i];
        if (current != NULL)
        {
            matrix_delete(&(current->matrix));
            free(current);
            (*m_list)->data[i] = NULL;
        }
    }
    free((*m_list)->data);
    free(*m_list);
    *m_list = NULL;
    return SUCCESS;
}

ErrorEnum matrix_list_add(MatrixList *m_list, Matrix *m1, char *name)
{
    assert(m_list != NULL);
    assert(m1 != NULL);
    assert(name != NULL);
    ErrorEnum error;
    /*Jeżeli istnieje już nazwa "name" w liście m_list, to zwróć FAILURE.
    Przekazujemy NULL w polu "returned_m", ponieważ nie potrzebujemy zwracać macierzy. */
    if (matrix_list_get(m_list, name, NULL) == SUCCESS)
        return MATRIX_ALREADY_EXISTS;

    int i = 0;
    while (i < m_list->size && m_list->data[i] != NULL)
    {
        i++;
    }
    if (i >= m_list->size)
    {
        CATCH_ERROR(matrix_list_resize(m_list));
    }

    ElementMatrix *new_element = malloc(sizeof(ElementMatrix));
    if (new_element == NULL)
        return MALLOC_ERROR;
    new_element->matrix = m1;
    strcpy((new_element->name), name);

    m_list->data[i] = new_element;
    return SUCCESS;
}

ErrorEnum matrix_list_get(const MatrixList *m_list, char *name, Matrix **returned_m)
{
    assert(m_list != NULL);
    assert(name != NULL);

    for (uint64_t i = 0; i < m_list->size; i++)
    {
        ElementMatrix *current = m_list->data[i];
        if (current != NULL && strcmp(current->name, name) == 0) // POPRAWKA: pełne dopasowanie
        {
            if (returned_m != NULL)
                *returned_m = current->matrix;
            return SUCCESS;
        }
    }
    if (returned_m != NULL)
        *returned_m = NULL;
    return MATRIX_NOT_FOUND;
}

ErrorEnum matrix_list_remove(MatrixList *m_list, char *name)
{
    assert(m_list != NULL);
    assert(name != NULL);

    for (int i = 0; i < m_list->size; i++)
    {
        ElementMatrix *current = m_list->data[i];
        if (current != NULL && strcmp(current->name, name) == 0) // POPRAWKA: == 0
        {
            matrix_delete(&(current->matrix));
            free(m_list->data[i]);
            m_list->data[i] = NULL;
            return SUCCESS;
        }
    }
    return MATRIX_NOT_FOUND;
}

int compare_names(const void *a, const void *b)
{
    const char *nameA = *((const char **)a);
    const char *nameB = *((const char **)b);

    return strcmp(nameA, nameB);
}

ErrorEnum matrix_list_names_sorted(const MatrixList *m_list, char ***list_of_names, uint64_t *store_amount)
{
    assert(m_list != NULL);
    assert(list_of_names != NULL);
    assert(store_amount != NULL);

    char **list_to_sort = calloc(sizeof(char *), m_list->size);
    int amount = 0;
    for (int i = 0; i < m_list->size; i++)
    {
        ElementMatrix *current = m_list->data[i];
        if (current != NULL)
        {
            list_to_sort[amount] = current->name;
            amount++;
        }
    }
    qsort(list_to_sort, amount, sizeof(char *), compare_names);

    *list_of_names = list_to_sort;
    *store_amount = amount;
    return SUCCESS;
}

ErrorEnum matrix_list_resize(MatrixList *m_list)
{
    assert(m_list != NULL);

    uint64_t old_size = m_list->size;
    uint64_t new_size = old_size * RESIZE_MULTIPLIER;
    ElementMatrix **resized_data = realloc(m_list->data, new_size * sizeof(ElementMatrix *));
    if (resized_data == NULL)
        return MALLOC_ERROR;
    m_list->size = new_size;

    for (uint64_t i = old_size; i < new_size; i++)
    {
        resized_data[i] = NULL;
    }
    m_list->data = resized_data;
    return SUCCESS;
}
