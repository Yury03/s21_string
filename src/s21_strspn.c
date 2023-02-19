#include "s21_string.h"

s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t rez = 0;
  char *ptr1 = (char *)str1;
  char *ptr2 = (char *)str2;
  for (; *ptr1 != '\0' && s21_strchr(ptr2, *ptr1++);) {
    rez++;
  }
  return rez;
}
