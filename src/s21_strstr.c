#include "s21_string.h"

int compare(const char *haystack, const char *needle) {
  int result = 0;
  while (*haystack && *needle) {
    if (*haystack != *needle) {
      result = 0;
      break;
    }

    haystack++;
    needle++;
  }
  result = (*needle == '\0');
  return result;
}

char *s21_strstr(const char *haystack, const char *needle) {
  const char *result = S21_NULL;
  int flag = 1;

  if (s21_strlen(haystack) == 0 && s21_strlen(needle) == 0) {
    flag = 0;
    result = "";
  }
  while (*haystack) {
    if ((*haystack == *needle && compare(haystack, needle)) ||
        (*haystack == '\0' || *needle == '\0')) {
      result = haystack;
      flag = 0;
      break;
    }
    haystack++;
  }

  return flag ? S21_NULL : (char *)result;
}
