#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  const char *pr = '\0';
  const char *pr1 = str;
  for (; *pr1 != '\0'; pr1++) {
    if (*pr1 == (char)c) {
      pr = pr1;
    }
  }
  if (*pr1 == '\0' && c == 0) {
    pr = pr1;
  }
  return (char *)pr;
}
