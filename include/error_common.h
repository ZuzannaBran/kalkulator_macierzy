#ifndef ERROR_COMMON_H
#define ERROR_COMMON_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\e[0;33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*!
 * @file error_common.h
 * @brief Moduł do obsługi błędów.
 */

///
/// Proste makro do obsługi błędów
///
#define CHECK_ERROR(func) \
    (error = func) != SUCCESS

#define CATCH_ERROR(func)      \
    do                         \
    {                          \
        if (CHECK_ERROR(func)) \
            return error;      \
    } while (0)
///
/// Enum, by ujednolicić zwracane błędy.
///
typedef enum error_enum
{
    SUCCESS,
    FAILURE,
    MALLOC_ERROR,
    OUT_OF_BOUNDS,
    FILE_OPENING_ERROR,
    FILE_CLOSING_ERROR,
    FILE_WRITE_ERROR,
    FILE_FORMAT_ERROR,
    FILE_READING_ERROR,
    FILE_SEEK_ERROR,
    MATRIX_FORMAT_ERROR,
    CMD_BAD_ARGS,
    EMPTY,
    MATRIX_NOT_FOUND,
    MATRIX_NAME_EXIST,
    MATRIX_ALREADY_EXISTS,
    NULL_POINTER_ERROR,
    MULTIPLY_ERROR,
    CONCAT_ERROR
    

} ErrorEnum;

///
/// Funkcja do wypisywania błędów
///
void print_cmd_error(int error);

#endif
