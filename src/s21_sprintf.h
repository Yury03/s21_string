#ifndef SRC_S21_SPRINTF_H
#define SRC_S21_SPRINTF_H
#include "s21_string.h"
#include <stdarg.h>
#include <stdint.h>
// #include <wchar.h>
#include <math.h>

typedef struct flags
{
    int minus;
    int plus;
    int sharp;
    int space;
    int zero;
    int width;
    int precision;
    int prec;
    int length;
    int specificator;
} flags;
// основные функции и парсеры
int s21_sprintf(char *str, const char *format, ...);
const char *firstParser(const char *str, flags *data);
const char *secondParser(const char *str, flags *data);
char *integers(char **str, flags a, va_list *ptr, unsigned int *count);
char *uintegers(char **str, flags a, va_list *ptr, unsigned int *count);
char *doubles(char **str, flags a, va_list *ptr, unsigned int *count);
char *chars(char **str, flags a, va_list *ptr, const char **format, unsigned int *count);
char *charPointers(char **str, flags a, va_list *ptr, const char **format, unsigned int *count);
char *pointers(char **str, flags a, va_list *ptr, unsigned int *count);
char *integerPointers(char **str, va_list *ptr, unsigned int count);
// вспомогательные функции
char *ourItoa(long long int value, char *result, int base);
char *ourUItoa(long long unsigned value, char *result, int base);
int stringToInteger(const char *str);
double stringToDouble(const char *s);
void cleanFlags(flags *data);
void widthHelper(char *result, flags data, int *i, int strlength);
void negativeSign(char *str, int i);
void positiveSign(flags data, char *str, int i);
#endif // SRC_S21_SPRINTF_H
