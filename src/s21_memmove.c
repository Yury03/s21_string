
#include "s21_string.h"

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  unsigned char *s21_dest = (unsigned char *)dest;
  unsigned char *s21_src = (unsigned char *)src;
  unsigned char temp[n];

  for (s21_size_t i = 0; i < n; i++) {
    temp[i] = s21_src[i];
  }
  for (s21_size_t i = 0; i < n; i++) {
    s21_dest[i] = temp[i];
  }
  return s21_dest;
}