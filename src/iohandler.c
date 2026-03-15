#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "error_common.h"
#include "matrix.h" 

ErrorEnum saveMatrix(Matrix *m, const char *filename)
{
    // Walidacja wskaźników wejściowych
    assert(m != NULL);
    assert(filename != NULL);

    // Walidacja struktury macierzy
    if (m->rows == 0 || m->columns == 0 || m->data == NULL){
        return MATRIX_FORMAT_ERROR;
    }

    FILE *f=fopen(filename, "wb");
    if (f == NULL) {
        return FILE_OPENING_ERROR;
    }

    // Zapis liczby wierszy
    if (fwrite(&m->rows, sizeof(m->rows), 1, f) != 1) {
        fclose(f);
        return FILE_WRITE_ERROR;
    }

    // Zapis danych macierzy
    size_t numOfELements = (size_t)m->columns * (size_t)m->rows;
    if (fwrite(m->data, sizeof(*m->data), numOfELements, f) != numOfELements) {
        fclose(f);
        return FILE_WRITE_ERROR;
    }

    if (fclose(f) != 0) {
        return FILE_CLOSING_ERROR;
    }
    
    return SUCCESS;
}

ErrorEnum loadMatrix(Matrix **m, const char *filename)
{
    assert(m!=NULL);
    assert(filename!=NULL);

    FILE *f = fopen (filename, "rb");
    if(f == NULL) {
        return FILE_OPENING_ERROR;
    }
    
    // Przejście do końca pliku w celu określenia jego rozmiaru
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return FILE_SEEK_ERROR;
    }

    long fileSize = ftell(f);
    if (fileSize < 0) {
        fclose(f);
        return FILE_SEEK_ERROR;
    }

    if (fileSize < (long)sizeof(uint64_t)) {
        fclose(f);
        return FILE_FORMAT_ERROR;
    }

    // Rozmiar danych (pomniejszony o nagłówek zawierający liczbę wierszy)
    long dataSize = fileSize - sizeof(uint64_t);

    if (dataSize == 0) {
        fclose(f);
        return FILE_FORMAT_ERROR;
    }

    // Sprawdzenie, czy rozmiar danych jest wielokrotnością rozmiaru float
    if (dataSize % sizeof(float) != 0) {
        fclose(f);
        return FILE_FORMAT_ERROR;
    }

    // Powrót na początek pliku
    if (fseek(f, 0, SEEK_SET) != 0) { 
        fclose(f);
        return FILE_SEEK_ERROR;
    }

    // Odczyt liczby wierszy
    uint64_t buf_rows;
    
    if (fread(&buf_rows, sizeof(uint64_t), 1, f) != 1) {
        fclose(f);
        return FILE_READING_ERROR;
    }

    if (buf_rows == 0) {
        fclose(f);
        return FILE_FORMAT_ERROR;
    }

    // Obliczenie liczby elementów
    uint64_t elementCount = dataSize / sizeof(float);

    // Sprawdzenie spójności: liczba elementów powinna być podzielna przez liczbę wierszy
    if (elementCount % buf_rows != 0) {
        fclose(f);
        return FILE_FORMAT_ERROR;
    }
    // Obliczenie liczby kolumn
    uint64_t buf_columns = elementCount / buf_rows;

    // Alokacja pamięci na dane macierzy (uwaga: potencjalny overflow)
    float *buf_data=malloc(elementCount * sizeof(float));
    if (buf_data == NULL) {
        fclose(f);
        return MALLOC_ERROR;
    }

    // Odczyt danych macierzy
    if (fread(buf_data, sizeof(float), elementCount, f) != elementCount) {
        fclose(f);
        free(buf_data);
        return FILE_READING_ERROR;
    }

    if (fclose(f) != 0) {
        return FILE_CLOSING_ERROR;
    }

    // Inicjalizacja struktury macierzy
    return matrix_init(buf_rows, buf_columns, buf_data, m);
}

ErrorEnum saveMatrixTxt(Matrix *m, const char *filename)
{
    // Walidacja wskaźników wejściowych
    assert(m != NULL);
    assert(filename != NULL);

    // Walidacja struktury macierzy
    if (m->rows == 0 || m->columns == 0 || m->data == NULL) {
        return MATRIX_FORMAT_ERROR;
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return FILE_OPENING_ERROR;
    }

    // Iteracja przez wszystkie elementy macierzy (wiersz po wierszu)
    for (uint64_t r = 0; r < m->rows; r++) {
        for (uint64_t c = 0; c < m->columns; c++) {

            // Pobranie wartości elementu
            float value;
            ErrorEnum err = matrix_get_value(r, c, m, &value);

            if (err != SUCCESS) {
                fclose(f);
                return err;
                }

            // Zapis wartości z precyzją 6 miejsc po przecinku
            if (fprintf(f, "%.6f", (double)value) < 0) {
                fclose(f);
                return FILE_WRITE_ERROR;
            }

            // Dodanie spacji między elementami (za wyjątkiem ostatniego w wierszu)
            if (c + 1 < m->columns) {
                if (fputc(' ', f) == EOF) {
                    fclose(f);
                    return FILE_WRITE_ERROR;
                }
            }
        }

        // Nowy wiersz po każdym wierszu macierzy
        if (fputc('\n', f) == EOF) {
            fclose(f);
            return FILE_WRITE_ERROR;
        }
    }

    // Sprawdzenie błędów strumienia (opcjonalne zabezpieczenie)
    if (ferror(f)) {
        fclose(f);
        return FILE_WRITE_ERROR;
    }

    // Uwaga: błąd fclose często oznacza flush failed => też traktujemy jako write error
    if (fclose(f) != 0) {
        return FILE_CLOSING_ERROR;
    }

    return SUCCESS;
}