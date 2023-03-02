#ifndef S21_STRINGS_H
#define S21_STRINGS_H
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define S21_NULL ((void *)0)
typedef unsigned long s21_size_t;
typedef struct flags {
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
// Выполняет поиск первого вхождения символа c (беззнаковый тип) в первых n
// байтах строки, на которую указывает аргумент str.
void *s21_memchr(const void *str, int c, s21_size_t n);

// Сравнивает первые n байтов str1 и str2.
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);

// Копирует n символов из src в dest.
void *s21_memcpy(void *dest, const void *src, s21_size_t n);

// Еще одна функция для копирования n символов из src в dest.
void *s21_memmove(void *dest, const void *src, s21_size_t n);

// Копирует символ c (беззнаковый тип) в первые n символов строки, на которую
// указывает аргумент str.
void *s21_memset(void *str, int c, s21_size_t n);

// Добавляет строку, на которую указывает src, в конец строки, на которую
// указывает dest.
char *s21_strcat(char *dest, const char *src);

// Добавляет строку, на которую указывает src, в конец строки, на которую
// указывает dest, длиной до n символов.
char *s21_strncat(char *dest, const char *src, s21_size_t n);

// Выполняет поиск первого вхождения символа c (беззнаковый тип) в строке, на
// которую указывает аргумент str.
char *s21_strchr(const char *str, int c);

// Сравнивает строку, на которую указывает str1, со строкой, на которую
// указывает str2.
int s21_strcmp(const char *str1, const char *str2);

// Сравнивает не более первых n байтов str1 и str2.
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);

// Копирует строку, на которую указывает src, в dest.
char *s21_strcpy(char *dest, const char *src);

// Копирует до n символов из строки, на которую указывает src, в dest.
char *s21_strncpy(char *dest, const char *src, s21_size_t n);

// Вычисляет длину начального сегмента str1, который полностью состоит из
// символов, не входящих в str2.
s21_size_t s21_strcspn(const char *str1, const char *str2);

// Выполняет поиск во внутреннем массиве номера ошибки errnum и возвращает
// указатель на строку с сообщением об ошибке. Нужно объявить макросы,
// содержащие массивы сообщений об ошибке для операционных систем mac и linux.
// Описания ошибок есть в оригинальной библиотеке. Проверка текущей ОС
// осуществляется с помощью директив.
char *s21_strerror(int errnum);

// Вычисляет длину строки str, не включая завершающий нулевой символ.
s21_size_t s21_strlen(const char *str);

// Находит первый символ в строке str1, который соответствует любому символу,
// указанному в str2.
char *s21_strpbrk(const char *str1, const char *str2);

// Выполняет поиск последнего вхождения символа c (беззнаковый тип) в строке, на
// которую указывает аргумент str.
char *s21_strrchr(const char *str, int c);

// Вычисляет длину начального сегмента str1, который полностью состоит из
// символов str2.
s21_size_t s21_strspn(const char *str1, const char *str2);

// Находит первое вхождение всей строки needle (не включая завершающий нулевой
// символ), которая появляется в строке haystack.
char *s21_strstr(const char *haystack, const char *needle);

// Разбивает строку str на ряд токенов, разделенных delim.
char *s21_strtok(char *str, const char *delim);

// допы
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);

// основные функции и парсеры  для спринтф
int s21_sprintf(char *str, const char *format, ...);
const char *firstParser(const char *str, flags *data);
const char *secondParser(const char *str, flags *data);
char *callSpecifFun(va_list *ptr, flags data, char **str, const char **format,
                    unsigned int *count);
char *integers(char **str, flags a, va_list *ptr, unsigned int *count);
char *uintegers(char **str, flags a, va_list *ptr, unsigned int *count);
char *doubles(char **str, flags a, va_list *ptr, unsigned int *count);
char *chars(char **str, flags a, va_list *ptr, const char **format,
            unsigned int *count);
char *charPointers(char **str, flags a, va_list *ptr, const char **format,
                   unsigned int *count);
char *wCharPointers(char **str, flags data,  // для работы с символами,
                    va_list *ptr,  // которые выходят за рамки ASCII
                    const char **format, unsigned int *count);
char *pointers(char **str, flags a, va_list *ptr, unsigned int *count);
char *integerPointers(char **str, va_list *ptr, unsigned int count);

// вспомогательные функции (в libs.c)
char *reverse(char s[]);
char *ourItoa(long long int value, char *result, int base);
char *ourUItoa(long long unsigned value, char *result, int base);
int stringToInteger(const char *str);
double stringToDouble(const char *s);
void cleanFlags(flags *data);
void widthHelper(char *result, flags data, int *i, int strlength);

void process_g(int *specificator, int *precision, char *nu, int *og,
               long double x);
void cut_off_zeros(char *nu, flags data, int og);
void to_upper(char *x);
void rounding_double(int length, int precision, char *nu, flags data);
void work_with_e(long double x, flags data, int precision, char **nu1, int og);
int value_to_str(long double x, char *nu, long double x1, int *length);
void double_to_str(long double x, long double x1, char *nu, int *length);
void recursive_long_int(long double d_arg, int symbols, char **str);
void e_to_str(long double x, char *nu, char *nunu, flags a, int *i,
              int precision);
void print_base(flags data, unsigned long x, char *result, int *i,
                int *precision);
void negativeSign(char *str, int i);
void positiveSign(flags data, char *str, int i);

#endif  // S21_STRING_H
