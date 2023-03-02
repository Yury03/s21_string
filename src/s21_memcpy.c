#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *s21_dest = (unsigned char *)dest;
  unsigned char *s21_src = (unsigned char *)src;
  if ((s21_dest != S21_NULL) && (s21_src != S21_NULL)) {
    for (s21_size_t i = 0; i < n; i++) {
      s21_dest[i] = s21_src[i];
    }
  }
  return s21_dest;
}