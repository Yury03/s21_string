#include "s21_sprintf.h"

char *reverse(char s[]) {  // нужно для Itoa
  char tmp;
  int i = 0, j = i - 1;

  for (i = 0, j = s21_strlen(s) - 1; i < j; i++, j--) {
    tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;
  }
  return s;
}

char *ourItoa(long long int value, char *result, int base) {
  int n = llabs(value);

  int i = 0;

  while (n) {
    int r = n % base;

    if (r >= 10) {
      result[i++] = 97 + (r - 10);
    } else {
      result[i++] = 48 + r;
    }

    n /= base;
  }

  if (i == 0) {
    result[i++] = '0';
  }

  if (value < 0) {
    result[i++] = '-';
  }

  result[i] = '\0';

  return reverse(result);
}

char *ourUItoa(long long unsigned value, char *result, int base) {
  long long unsigned n = value;

  int i = 0;

  while (n) {
    int r = n % base;

    if (r >= 10) {
      result[i++] = 97 + (r - 10);
    } else {
      result[i++] = 48 + r;
    }

    n /= base;
  }

  if (i == 0) {
    result[i++] = '0';
  }

  result[i] = '\0';

  return reverse(result);
}

int stringToInteger(const char *str) {  // фактически, это atoi
  int result = 0;
  while (*str >= '0' && *str <= '9') {
    result *= 10;
    result += *str++;
    result -= '0';
  }

  return result;
}

double stringToDouble(const char *s) {
  const char *ps = s;
  double result = 0;
  double fact = 1;

  if (*ps == '-') {
    fact = -fact;
    ps++;
  }

  for (int point = 0; *ps; ps++) {
    if (*ps == '.') {
      point = 1;
      continue;
    }

    int tmp = *ps - '0';
    if (tmp >= 0 && tmp <= 9) {
      if (point) fact /= 10.;
      result = result * 10. + (double)tmp;
    }

    if (*ps <= '0' || *ps >= '9') {
      break;
    }
  }

  return result * fact;
}

void cleanFlags(flags *data) {
  data->length = 0;
  data->minus = 0;
  data->plus = 0;
  data->precision = 0;
  data->sharp = 0;
  data->space = 0;
  data->specificator = 0;
  data->prec = 0;
  data->width = 0;
  data->zero = 0;
}

void widthHelper(char *result, flags data, int *i, int strlength) {
  // если в этом месте нужно печатать ширину
  while (
      !data.minus &&
      (data.width - (data.precision > strlength ? data.precision : strlength) >
       0)) {
    if (!data.prec && data.zero)
      result[(*i)] = '0';  // печатаем нулями
    else
      result[(*i)] = ' ';  // или пробелами, в зависимости от флагов
    (*i)++;
    data.width--;
  }
}

void negativeSign(char *str, int i) {
  int n = 0;
  if (!(str[0] == '0' || i == 0))
    n = i - 1;  // смотрим, куда нужно ставить знак
  str[n] = '-';
}

void positiveSign(flags data, char *str, int i) {
  int n = 0;
  if (!(str[0] == '0' || i == 0))
    n = i - 1;  // смотрим, куда нужно ставить знак
  if (data.plus)
    str[n] = '+';  // тоже самое
  else if (data.space)
    str[n] = ' ';  // или пробел, если есть флаг пробелаZ
}