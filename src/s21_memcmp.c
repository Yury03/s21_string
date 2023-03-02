#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *line_1 = str1;
  const unsigned char *line_2 = str2;
  s21_size_t res = 0;
  while (n--) {
    if (*line_1 != *line_2) {
      res = *line_1 - *line_2;
      break;
    }
    line_1++;
    line_2++;
  }
  return res;
}