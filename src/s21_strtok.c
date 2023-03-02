#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *next_symbol;

  if (str == S21_NULL && *delim == ' ') {
    str = S21_NULL;
  } else {
    if (str) {
      next_symbol = str;
      while (*next_symbol && s21_strchr(delim, *next_symbol)) {
        next_symbol++;
      }
    }

    if (!*next_symbol) {
      str = S21_NULL;
    } else {
      str = next_symbol;

      while (*next_symbol && !s21_strchr(delim, *next_symbol)) ++next_symbol;
      while (*next_symbol && s21_strchr(delim, *next_symbol)) {
        *next_symbol = '\0';
        next_symbol++;
      }
    }
  }

  return str;
}