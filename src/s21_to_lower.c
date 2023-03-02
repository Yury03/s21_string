#include "s21_string.h"

void *s21_to_lower(const char *str) {
  char *up = S21_NULL;
  if (str) {
    up = calloc(s21_strlen(str) + 2, sizeof(char));
    if (up != S21_NULL) {
      for (int i = 0; str[i]; i++) {
        if (str[i] >= 65 && str[i] <= 90) {
          up[i] = str[i] + 32;
        } else {
          up[i] = str[i];
        }
      }
    }
  }
  return (void *)up;
}