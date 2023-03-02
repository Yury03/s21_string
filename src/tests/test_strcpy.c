#include "s21_test.h"

START_TEST(test_s21_strcpy) {
  char *dest1 = calloc(100, sizeof(char));
  char *dest2 = calloc(100, sizeof(char));
  char *str1 = "AbraCadabra";
  char *str2 = "Hello World";
  char *str3 = "Python sucks";
  dest1 = strcpy(dest1, str1);
  dest2 = s21_strcpy(dest2, str1);
  ck_assert_str_eq(dest1, dest2);

  dest1 = strcpy(dest1, str2);
  dest2 = s21_strcpy(dest2, str2);
  ck_assert_str_eq(dest1, dest2);

  dest1 = strcpy(dest1, str3);
  dest2 = s21_strcpy(dest2, str3);
  ck_assert_str_eq(dest1, dest2);

  free(dest1);
  free(dest2);
}
END_TEST

Suite *test_strcpy(void) {
  Suite *s = suite_create("\033[45m-=S21_STRCPY=-\033[0m");
  TCase *tc = tcase_create("strcpy_tc");

  suite_add_tcase(s, tc);
  tcase_add_test(tc, test_s21_strcpy);

  suite_add_tcase(s, tc);
  return s;
}
