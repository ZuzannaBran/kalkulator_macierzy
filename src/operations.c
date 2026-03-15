#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<assert.h>

#include "../include/operations.h"
#include "../include/matrix.h"
#include "../include/error_common.h"

/*
funkcja pomocnicza wewnętrzna Tworzy “pustą” macierz o wymiarach rows x cols.
alokuje pamięć na dane (float*),
zeruje tablicę.
Zwraca wskaźnik na utworzoną macierz.
*/
static ErrorEnum create_empty_matrix(uint64_t rows, uint64_t cols, Matrix **out)
{
    assert(out != NULL);
    *out = NULL;

    //Nie można tworzyć macierzy o zerowym rozmiarze - polecenie
    if (rows == 0 || cols == 0) return OUT_OF_BOUNDS;

    //Sprawdzenie przepełnienia przy mnożeniu
    uint64_t element_count = rows * cols;
    if (rows != 0 && element_count / rows != cols) return FAILURE; 

    //Liczymy liczbę bajtów do alokacji, sprawdzamy przepelnienie
    size_t bytes = (size_t)element_count * sizeof(float);
    if (element_count != 0 && bytes / sizeof(float) != (size_t)element_count) return FAILURE;

    //Alokacja pamięci na dane
    float *data = (float *)malloc(bytes);
    if (data == NULL) return MALLOC_ERROR;

    //Zerowanie pamięci
    memset(data, 0, bytes);

    //Tworzymy strukturę Matrix i przypinamy wskaźnik data
    ErrorEnum error;
    if(CHECK_ERROR(matrix_init(rows, cols, data, out)))
    {
        free(data);
        return error;
    }
    
    return SUCCESS;
}



ErrorEnum matrix_subtract(const Matrix *a, const Matrix *b, Matrix **result)
{
    //Sprawdzanie danych wejściowych
    assert(result != NULL);
    *result = NULL;
    assert(a != NULL && b != NULL);
    assert(a->data != NULL && b->data != NULL);


    //Sprawdzenie zgodności rozmiarów, musi być taka sama
    if (a->rows != b->rows || a->columns != b->columns) return FAILURE;

    //Utworzenie macierzy wynikowej
    Matrix *out = NULL;
    ErrorEnum err = create_empty_matrix(a->rows, a->columns, &out);
    if (err != SUCCESS) return err;

    /*
    odejmowanie element po elemencie
    element (i,j) jest pod indeksem i*columns + j
    Ponieważ a i b mają ten sam rozmiar, możemy iść liniowo po wszystkich elementach
    */
    uint64_t total = a->rows * a->columns;
    for (uint64_t idx = 0; idx < total; idx++)
    {
        out->data[idx] = a->data[idx] - b->data[idx];
    }

    *result = out;
    return SUCCESS;
}

ErrorEnum matrix_multiply(const Matrix *a, const Matrix *b, Matrix **result)
{
    assert(result != NULL);
    *result = NULL;
    assert(a != NULL && b != NULL);
    assert(a->data != NULL && b->data != NULL);

    //Warunek mnożenia macierzy 
    if (a->columns != b->rows) return MULTIPLY_ERROR;

    //Tworzenie macierzy wynikowej
    Matrix *out = NULL;
    ErrorEnum error;
    CATCH_ERROR(create_empty_matrix(a->rows, b->columns, &out));

    //Mnożenie macierzy, używamy double dla akumulacji, by zmniejszyć błędy zaokrągleń.
    for (uint64_t i = 0; i < a->rows; i++)
    {
        for (uint64_t j = 0; j < b->columns; j++)
        {
            double acc = 0.0; //Podwójna precyzja
            for (uint64_t k = 0; k < a->columns; k++)
            {
                
                float aval;
                if(CHECK_ERROR(matrix_get_value(i,k,a,&aval)))
                {
                    matrix_delete(&out);
                    return error;
                }
                float bval; 
                if(CHECK_ERROR(matrix_get_value(k,j,b,&bval)))
                {
                    matrix_delete(&out);
                    return error;
                }
                acc += (double)aval * (double)bval;
            }
            if(CHECK_ERROR(matrix_set_value(i,j,out,(float)acc)))
            {
                matrix_delete(&out);
                return error;
            }
        }
    }

    *result = out;
    return SUCCESS;
}

ErrorEnum matrix_concat_horizontal(const Matrix *left, const Matrix *right, Matrix **result)
{
    //Sprawdzenie wskaźników
    assert(result != NULL);
    assert(left != NULL && right != NULL);
    assert(left->data != NULL && right->data != NULL);

    //Łączenie poziome wymaga tej samej liczby wierszy
    if (left->rows != right->rows) return FAILURE;

    uint64_t new_rows = left->rows;
    uint64_t new_cols = left->columns + right->columns;

    //Sprawdzenie przepełnienia dla dodawania kolumn
    if (new_cols < left->columns) return FAILURE;

    Matrix *out = NULL;
    ErrorEnum err = create_empty_matrix(new_rows, new_cols, &out);
    if (err != SUCCESS) return err;

    //dane są ułożone kolumna po kolumnie,
    //sklejamy dwa ciągi floatów left+right
    const uint64_t left_elems  = left->rows * left->columns;
    const uint64_t right_elems = right->rows * right->columns;


    memcpy(out->data, left->data, (size_t)left_elems * sizeof(float));
    memcpy(out->data + left_elems, right->data, (size_t)right_elems * sizeof(float));
    
    *result = out;
    return SUCCESS;
}
