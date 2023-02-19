#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *result = S21_NULL;
  if (s21_strlen(src) >= start_index && src) {
    result =
        (char *)calloc((s21_strlen(src) + s21_strlen(str) + 1), sizeof(char));
    if (result) {
      s21_size_t i = 0;
      s21_size_t j = 0;
      for (; i < start_index; i++) {
        result[i] = src[i];
      }
      for (; j < s21_strlen(str); j++) {
        result[i + j] = str[j];
      }
      for (i = start_index; i < s21_strlen(src); i++) {
        result[i + j] = src[i];
      }
    }
  }
  return result;
}
