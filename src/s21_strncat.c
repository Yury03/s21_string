#include "s21_string.h"

char *s21_strncat(char *destination, const char *append, s21_size_t n) {
  s21_size_t dest = s21_strlen(destination);
  s21_size_t i = 0;
  while (i < n) {
    destination[dest] = append[i];
    i++;
    dest++;
  }
  return destination;
}