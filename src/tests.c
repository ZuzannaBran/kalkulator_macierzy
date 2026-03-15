#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "matrix_list.h"
#include "iohandler.h"
#include "error_common.h"
#include "operations.h"
#include "script.h"


/*
----------------JAK TESTUJEMY KOD----------------
Jeżeli chcemy przeprowadzić wszystkie testy wpisujemy w terminal komende 'make test'

Jeżeli chcemy dodać funkcje testującą nasz program piszemy ją nastepująco
Na początku funkcji wywołujemy TEST_START()

Każda funkcja która może zwrócić ErrorEnum opakowujemy w TEST_PROGRAM_ERROR()
Jeżeli porównujemy wartości, to robić to przy użyciu TEST_EQUAL()

zwracamy watość 1 lub 0, w zależności czy był błąd czy nie
*/

///
///ZAWSZE wywoływać na starcie funkcji testujących
///
#define TEST_START() \
    ErrorEnum error; \
    int czy_blad_testow = 0;
/*!
 * Macro służy do sprawdzania błędow programu z kategori ErrorEnum.
 * Przydatna, bo jak testujemy, to mogą nam różne błędy zwrócić np malloc czy out of bounds i wtedy dalej testów nie możemy prowadzić
 * w danej funkcji testującej. nr_blad_program służy, by sprawdzać gdzie zaszedł dany błąd w funkcji testującej.
 */
#define TEST_PROGRAM_ERROR(func)\
    if((error = func)) {\
        puts(ANSI_COLOR_RED "\tBlad programu"ANSI_COLOR_RESET" funkcja "#func);\
        return 1;\
    }\
///
///Sprawdzamy rowność wartości. nr_test_equal służy, by sprawdzać gdzie zaszedł dany błąd w funkcji testującej.
///
#define TEST_EQUAL(func_value,expected_value) \
    do {\
    if((func_value)!=(expected_value)) {\
        puts(ANSI_COLOR_RED "\tBlad testu "ANSI_COLOR_RESET"TEST_EQUAL("#func_value","#expected_value")");\
        czy_blad_testow=1;\
    }} while(0)

#define FLOAT_ABS(x) ((x) < 0.0f ? -(x) : (x))

///
///Porównanie floatów z tolerancją (przydatne np. dla saveMatrixTxt)
///
#define TEST_FLOAT_APPROX(actual, expected, eps) \
    do { \
        float __diff = (actual) - (expected); \
        if (FLOAT_ABS(__diff) > (eps)) { \
            printf(ANSI_COLOR_RED "\tBlad testu " ANSI_COLOR_RESET "(%.9g != %.9g)\n", (double)(actual), (double)(expected)); \
            czy_blad_testow = 1; \
        } \
    } while(0)

///
///Do wywoływania funkcji testujacych program, zwraca informacje o ich powodzeniu
///
#define TEST_CHECK(test_func)\
    puts(ANSI_COLOR_YELLOW"Sprawdzam "ANSI_COLOR_RESET #test_func );\
    if(!test_func()) puts("\t" ANSI_COLOR_GREEN "Sukces" ANSI_COLOR_RESET);\
        else puts("\t" ANSI_COLOR_RED "Niepowodzenie" ANSI_COLOR_RESET  )

ErrorEnum testowa_macierz(Matrix** m1)
{
    float *data = calloc(sizeof(float),6);
    if (data ==NULL)
    {
        return MALLOC_ERROR;
    }

    data[0]=1.21f;data[3]=6.1f;
    data[1]=4.14f;data[4]=0;
    data[2]=0.12f;data[5]=123.31f;
    return matrix_init(3,2,data,m1);
}

int matrix_test0(void)
{
    TEST_START();
    Matrix *m1;
    float value;
    TEST_PROGRAM_ERROR(testowa_macierz(&m1));

    TEST_PROGRAM_ERROR(matrix_get_value(2,1,m1,&value));
    TEST_EQUAL(value,123.31f);

    TEST_PROGRAM_ERROR(matrix_set_value(2,1,m1,5431.1f));
    TEST_PROGRAM_ERROR(matrix_get_value(2,1,m1,&value));
    TEST_EQUAL(value,5431.1f);

    matrix_delete(&m1);
    TEST_EQUAL(m1,NULL);
    return czy_blad_testow;
}

int matrix_list_test0(void)
{
    TEST_START();
    MatrixList *m_list;
    TEST_PROGRAM_ERROR(matrix_list_init(&m_list));
    Matrix *m1;
    Matrix *m_temp;
    TEST_PROGRAM_ERROR(testowa_macierz(&m1));
    TEST_PROGRAM_ERROR(matrix_list_add(m_list,m1,"Abc"));
    TEST_PROGRAM_ERROR(matrix_list_get(m_list,"Abc",&m_temp));

    float value;
    TEST_PROGRAM_ERROR(matrix_get_value(1,0,m_temp,&value));
    TEST_EQUAL(value,4.14f);

    for (int i = 0; i < 9; i++)
    {
        char nazwa[3];
        nazwa[0] = 'a';nazwa[1]='0'+i;nazwa[2]=0;
        Matrix *m_loop;
        TEST_PROGRAM_ERROR(testowa_macierz(&m_loop));
        TEST_PROGRAM_ERROR(matrix_list_add(m_list,m_loop,nazwa));
    }
    TEST_EQUAL(m_list->size,16);

    matrix_list_delete(&m_list);
    TEST_EQUAL(m_list,NULL);
    return czy_blad_testow;
}
int matrix_list_test1(void)
{
    TEST_START();
    MatrixList *m_list;
    TEST_PROGRAM_ERROR(matrix_list_init(&m_list));
    Matrix *m_loop;
    TEST_PROGRAM_ERROR(testowa_macierz(&m_loop));
    TEST_PROGRAM_ERROR(matrix_list_add(m_list,m_loop,"fbecede"));
    TEST_PROGRAM_ERROR(testowa_macierz(&m_loop));
    TEST_PROGRAM_ERROR(matrix_list_add(m_list,m_loop,"erfawea"));
    TEST_PROGRAM_ERROR(testowa_macierz(&m_loop));
    TEST_PROGRAM_ERROR(matrix_list_add(m_list,m_loop, "aaa"));
    TEST_PROGRAM_ERROR(testowa_macierz(&m_loop));
    TEST_PROGRAM_ERROR(matrix_list_add(m_list,m_loop,"zerfaew"));
    TEST_PROGRAM_ERROR(testowa_macierz(&m_loop));
    TEST_PROGRAM_ERROR(matrix_list_add(m_list,m_loop,"zewa"));
     TEST_PROGRAM_ERROR(testowa_macierz(&m_loop));
    TEST_PROGRAM_ERROR(matrix_list_add(m_list,m_loop,"bbbbbbbbbbbbbbbbbb"));

    char** names_list;
    uint64_t size;
    TEST_PROGRAM_ERROR(matrix_list_names_sorted(m_list,&names_list,&size));
    TEST_EQUAL(names_list[0][0],'a');
    TEST_EQUAL(names_list[1][0],'b');
    TEST_EQUAL(names_list[3][0],'f');
    TEST_EQUAL(names_list[5][2],'w');
    
    matrix_list_delete(&m_list);
    return czy_blad_testow;
}
int operations_test_subtract(void)
{
    TEST_START();
    Matrix *a = NULL, *b = NULL, *res = NULL;

    TEST_PROGRAM_ERROR(testowa_macierz(&a));
    TEST_PROGRAM_ERROR(testowa_macierz(&b));

    TEST_PROGRAM_ERROR(matrix_subtract(a,b,&res));

    float v;
    TEST_PROGRAM_ERROR(matrix_get_value(0,0,res,&v));  TEST_EQUAL(v,0.0f);
    TEST_PROGRAM_ERROR(matrix_get_value(0,1,res,&v));  TEST_EQUAL(v,0.0f);
    TEST_PROGRAM_ERROR(matrix_get_value(2,0,res,&v));  TEST_EQUAL(v,0.0f);
    TEST_PROGRAM_ERROR(matrix_get_value(2,1,res,&v));  TEST_EQUAL(v,0.0f);

    matrix_delete(&a);
    matrix_delete(&b);
    matrix_delete(&res);

    return czy_blad_testow;
}

int operations_test_multiply(void)
{
    TEST_START();

    Matrix *a = NULL, *b = NULL, *res = NULL;

    TEST_PROGRAM_ERROR(testowa_macierz(&a));

    float *bdata = malloc(2 * 3 * sizeof(float));
    if (bdata == NULL) return 1;
    bdata[0] = 1; bdata[2] = 2; bdata[4] = 3;
    bdata[1] = 4; bdata[3] = 5; bdata[5] = 6;
    TEST_PROGRAM_ERROR(matrix_init(2, 3, bdata, &b));

    TEST_PROGRAM_ERROR(matrix_multiply(a, b, &res));

    float v;
    TEST_PROGRAM_ERROR(matrix_get_value(0,0,res,&v)); TEST_EQUAL(v,25.61f);
    TEST_PROGRAM_ERROR(matrix_get_value(0,1,res,&v)); TEST_EQUAL(v,32.92f);
    TEST_PROGRAM_ERROR(matrix_get_value(1,0,res,&v)); TEST_EQUAL(v,4.14f);
    TEST_PROGRAM_ERROR(matrix_get_value(2,2,res,&v)); TEST_EQUAL(v,740.22f);

    matrix_delete(&a);
    matrix_delete(&b);
    matrix_delete(&res);

    return czy_blad_testow;
}

int operations_test_concat_horizontal(void)
{
    TEST_START();
    Matrix *left = NULL, *right = NULL, *res = NULL;

    TEST_PROGRAM_ERROR(testowa_macierz(&left));
    TEST_PROGRAM_ERROR(testowa_macierz(&right));

    TEST_PROGRAM_ERROR(matrix_concat_horizontal(left, right, &res));

    float v;
    TEST_PROGRAM_ERROR(matrix_get_value(0,0,res,&v));  TEST_EQUAL(v,1.21f);
    TEST_PROGRAM_ERROR(matrix_get_value(0,1,res,&v));  TEST_EQUAL(v,6.1f);
    TEST_PROGRAM_ERROR(matrix_get_value(0,2,res,&v));  TEST_EQUAL(v,1.21f);
    TEST_PROGRAM_ERROR(matrix_get_value(0,3,res,&v));  TEST_EQUAL(v,6.1f);
    TEST_PROGRAM_ERROR(matrix_get_value(2,3,res,&v));  TEST_EQUAL(v,123.31f);

    matrix_delete(&left);
    matrix_delete(&right);
    matrix_delete(&res);

    return czy_blad_testow;
}

/* =======================
   TESTY IOHANDLER (NOWE)
   ======================= */

static int iohandler_test0_save_load_roundtrip(void)
{
    TEST_START();

    const char *filename = "tmp_matrix_iohandler.bin";

    Matrix *m1 = NULL;
    TEST_PROGRAM_ERROR(testowa_macierz(&m1));

    TEST_PROGRAM_ERROR(saveMatrix(m1, filename));

    Matrix *m2 = NULL;
    TEST_PROGRAM_ERROR(loadMatrix(&m2, filename));

    TEST_EQUAL((m2 != NULL), 1);
    TEST_EQUAL(m2->rows, m1->rows);
    TEST_EQUAL(m2->columns, m1->columns);
    TEST_EQUAL((m2->data != NULL), 1);

    for (unsigned long long i = 0; i < m1->rows; i++) {
        for (unsigned long long j = 0; j < m1->columns; j++) {
            float v1 = 0.0f, v2 = 0.0f;
            TEST_PROGRAM_ERROR(matrix_get_value(i, j, m1, &v1));
            TEST_PROGRAM_ERROR(matrix_get_value(i, j, m2, &v2));
            TEST_EQUAL(v2, v1); // binarnie powinno być identyczne
        }
    }

    matrix_delete(&m1);
    matrix_delete(&m2);
    remove(filename);

    return czy_blad_testow;
}

static int iohandler_test1_save_errors(void)
{
    TEST_START();

    static float dummy = 0.0f;

    Matrix *good = NULL;
    TEST_PROGRAM_ERROR(testowa_macierz(&good));

    // UWAGA: saveMatrix ma assert() na NULL, więc NIE testujemy NULL wskaźników.

    Matrix bad1 = {.rows = 0, .columns = 2, .data = &dummy};
    error = saveMatrix(&bad1, "x.bin");
    TEST_EQUAL(error, MATRIX_FORMAT_ERROR);

    Matrix bad2 = {.rows = 2, .columns = 0, .data = &dummy};
    error = saveMatrix(&bad2, "x.bin");
    TEST_EQUAL(error, MATRIX_FORMAT_ERROR);

    Matrix bad3 = {.rows = 2, .columns = 2, .data = NULL};
    error = saveMatrix(&bad3, "x.bin");
    TEST_EQUAL(error, MATRIX_FORMAT_ERROR);

#ifdef _WIN32
    error = saveMatrix(good, "Z:/__this_path_should_not_exist__/x.bin");
    TEST_EQUAL(error, FILE_OPENING_ERROR);
#else
    // zapis do katalogu powinien się nie udać
    error = saveMatrix(good, "/");
    TEST_EQUAL(error, FILE_OPENING_ERROR);
#endif

    matrix_delete(&good);
    return czy_blad_testow;
}

static int iohandler_test2_load_errors(void)
{
    TEST_START();
    // UWAGA: loadMatrix ma assert() na NULL, więc NIE testujemy NULL wskaźników.

    Matrix *m = NULL;

    error = loadMatrix(&m, "__this_file_should_not_exist__.bin");
    TEST_EQUAL(error, FILE_OPENING_ERROR);
    TEST_EQUAL((m == NULL), 1);

    {
        const char *fn = "bad_small.bin";
        FILE *f = fopen(fn, "wb");
        if(!f) return 1;
        fclose(f);

        error = loadMatrix(&m, fn);
        TEST_EQUAL(error, FILE_FORMAT_ERROR);
        TEST_EQUAL((m == NULL), 1);
        remove(fn);
    }

    {
        const char *fn = "bad_header_only.bin";
        FILE *f = fopen(fn, "wb");
        if(!f) return 1;
        unsigned long long rows = 2;
        fwrite(&rows, sizeof(rows), 1, f);
        fclose(f);

        error = loadMatrix(&m, fn);
        TEST_EQUAL(error, FILE_FORMAT_ERROR);
        TEST_EQUAL((m == NULL), 1);
        remove(fn);
    }

    {
        const char *fn = "bad_datasize.bin";
        FILE *f = fopen(fn, "wb");
        if(!f) return 1;
        unsigned long long rows = 2;
        fwrite(&rows, sizeof(rows), 1, f);
        unsigned char junk[3] = {1,2,3};
        fwrite(junk, 1, sizeof(junk), f);
        fclose(f);

        error = loadMatrix(&m, fn);
        TEST_EQUAL(error, FILE_FORMAT_ERROR);
        TEST_EQUAL((m == NULL), 1);
        remove(fn);
    }

    {
        const char *fn = "bad_rows0.bin";
        FILE *f = fopen(fn, "wb");
        if(!f) return 1;
        unsigned long long rows = 0;
        float x = 1.0f;
        fwrite(&rows, sizeof(rows), 1, f);
        fwrite(&x, sizeof(x), 1, f);
        fclose(f);

        error = loadMatrix(&m, fn);
        TEST_EQUAL(error, FILE_FORMAT_ERROR);
        TEST_EQUAL((m == NULL), 1);
        remove(fn);
    }

    {
        const char *fn = "bad_not_divisible.bin";
        FILE *f = fopen(fn, "wb");
        if(!f) return 1;
        unsigned long long rows = 3;
        float buf[4] = {1.0f,2.0f,3.0f,4.0f};
        fwrite(&rows, sizeof(rows), 1, f);
        fwrite(buf, sizeof(float), 4, f);
        fclose(f);

        error = loadMatrix(&m, fn);
        TEST_EQUAL(error, FILE_FORMAT_ERROR);
        TEST_EQUAL((m == NULL), 1);
        remove(fn);
    }

    return czy_blad_testow;
}

static int iohandler_test3_save_txt_roundtrip(void)
{
    TEST_START();

    const char *fn = "tmp_matrix_iohandler.txt";

    Matrix *m1 = NULL;
    TEST_PROGRAM_ERROR(testowa_macierz(&m1));

    TEST_PROGRAM_ERROR(saveMatrixTxt(m1, fn));

    FILE *f = fopen(fn, "r");
    if (!f) {
        matrix_delete(&m1);
        return 1;
    }

    for (unsigned long long r = 0; r < m1->rows; r++) {
        for (unsigned long long c = 0; c < m1->columns; c++) {
            float expected = 0.0f;
            float readv = 0.0f;

            TEST_PROGRAM_ERROR(matrix_get_value(r, c, m1, &expected));

            if (fscanf(f, "%f", &readv) != 1) {
                fclose(f);
                matrix_delete(&m1);
                return 1;
            }

            // w txt mamy %.6f, więc bierzemy tolerancję
            TEST_FLOAT_APPROX(readv, expected, 1e-5f);
        }
    }

    // po wszystkich liczbach powinien być EOF
    float extra = 0.0f;
    int ret = fscanf(f, "%f", &extra);
    TEST_EQUAL(ret, EOF);

    fclose(f);
    remove(fn);
    matrix_delete(&m1);

    return czy_blad_testow;
}

static int iohandler_test4_save_txt_errors(void)
{
    TEST_START();

    static float dummy = 0.0f;

    Matrix *good = NULL;
    TEST_PROGRAM_ERROR(testowa_macierz(&good));

    // UWAGA: saveMatrixTxt ma assert() na NULL, więc NIE testujemy NULL wskaźników.

    Matrix bad1 = {.rows = 0, .columns = 2, .data = &dummy};
    error = saveMatrixTxt(&bad1, "x.txt");
    TEST_EQUAL(error, MATRIX_FORMAT_ERROR);

    Matrix bad2 = {.rows = 2, .columns = 0, .data = &dummy};
    error = saveMatrixTxt(&bad2, "x.txt");
    TEST_EQUAL(error, MATRIX_FORMAT_ERROR);

    Matrix bad3 = {.rows = 2, .columns = 2, .data = NULL};
    error = saveMatrixTxt(&bad3, "x.txt");
    TEST_EQUAL(error, MATRIX_FORMAT_ERROR);

#ifdef _WIN32
    error = saveMatrixTxt(good, "Z:/__this_path_should_not_exist__/x.txt");
    TEST_EQUAL(error, FILE_OPENING_ERROR);
#else
    error = saveMatrixTxt(good, "/");
    TEST_EQUAL(error, FILE_OPENING_ERROR);
#endif

    matrix_delete(&good);
    return czy_blad_testow;
}

int scripted_test0()
{
    TEST_START();
    Matrix *m;
    TEST_PROGRAM_ERROR(testowa_macierz(&m));
    TEST_PROGRAM_ERROR(saveMatrix(m,"test1.mx"));
    Matrix *m2;
    float *data;
    data = malloc(6*sizeof(float));
    data[0]=1.2; data[2]=10; data[4] =2;
    data[1]=5.1; data[3]=1;  data[5] =3;
    TEST_PROGRAM_ERROR(matrix_init(2,3,data,&m2));
    TEST_PROGRAM_ERROR(saveMatrix(m2,"test2.mx"));
    MatrixList *m_list;
    TEST_PROGRAM_ERROR(matrix_list_init(&m_list));
    TEST_PROGRAM_ERROR(run_script_from_file(m_list,"sktest0.txt"));
    
    TEST_PROGRAM_ERROR(matrix_list_delete(&m_list));
    return czy_blad_testow;
}
int main(void)
{
    TEST_CHECK(matrix_test0);
    TEST_CHECK(matrix_list_test0);
    TEST_CHECK(matrix_list_test1);
    TEST_CHECK(operations_test_subtract);
    TEST_CHECK(operations_test_multiply);
    TEST_CHECK(operations_test_concat_horizontal);

    TEST_CHECK(iohandler_test0_save_load_roundtrip);
    TEST_CHECK(iohandler_test1_save_errors);
    TEST_CHECK(iohandler_test2_load_errors);
    TEST_CHECK(iohandler_test3_save_txt_roundtrip);
    TEST_CHECK(iohandler_test4_save_txt_errors);
    TEST_CHECK(scripted_test0);

    return 0;
}
