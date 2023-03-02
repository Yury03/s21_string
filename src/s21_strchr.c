#include "s21_string.h"

char *s21_strchr(const char *str1, int c) {
  char *res = S21_NULL;
  while (*str1 != c && *str1) str1++;  //  первое вхождение с в str1
  return res = (c == *str1) ? (char *)str1 : res;
}  // str1[i] если 'c' == str1 иначе NULL