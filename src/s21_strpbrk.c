#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  int flag = 0;
  s21_size_t i;
  s21_size_t c_str1 = s21_strlen(str1);
  s21_size_t c_str2 = s21_strlen(str2);
  for (i = 0; i < c_str1 && !flag; i++) {
    for (s21_size_t j = 0; j < c_str2 && !flag; j++) {
      if (str1[i] == str2[j]) {  // todo должно работать
        flag = 1;
        break;
      }
    }
  }
  i--;

  return (flag == 1) ? (char *)&str1[i] : S21_NULL;
}
