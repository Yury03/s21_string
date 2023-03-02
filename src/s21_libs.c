#include "s21_string.h"

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

void print_base(flags data, unsigned long x, char *result, int *i,
                int *precision) {
  if (data.sharp && x != 0) {  // если надо написать базу числа
    int base = 0;  // тут будем отмечать позицию для записи базы
    if (data.specificator == 'o') {  // если число восьмеричное
      if ((*i) > 0 && result[(*i) - 1] == 32)
        base = (*i) - 1;  // если базу надо писать не в начало
      if (base == 0) {
        (*i)++;  // сдвигаемся по строке, потому что сюда уже будем писать базу
      }
      result[base] = '0';  // записали базу в нужно место
      (*precision)--;  // у восьмеричного числа база учитывается в точности
    } else if (data.specificator == 'x' ||
               data.specificator == 'X') {  // если 16-ричное
      if ((*i) > 1 && result[(*i) - 1] ==
                          32) {  // если мы можем взять оба символа из ширины
        base = (*i) - 2;  // то запишем базу в счёт ширины
      } else if ((*i) == 1 &&
                 result[(*i) - 1] == 32) {  // тут заберём один символ из ширины
        (*i)++;  // поэтому сдвиг для дальнейшей записи будет в один символ
      } else {  // тут не сможем взять из ширины, оба символа базы запишутся с
                // текущей позиции
        (*i) += 2;  // поэтому для дальнейшей записи сдвигаемся на 2
      }
      result[base] = '0';  // первый символ 16-ричной базы - 0
      result[base + 1] = data.specificator;  // второй - x или X, будет
                                             // соответствовать спецификатору
    }
  }
}

/////-----------------для doubles

void process_g(int *specificator, int *precision, char *nu, int *og,
               long double x) {
  if (*specificator == 'e' || *specificator == 'E' || *specificator == 'g' ||
      *specificator == 'G') {
    if (*specificator == 'g' || *specificator == 'G') *og = 1;
    int expo = 0, expo_help = 1;
    if ((x < 10 && x > -10) && fmodl(x, 1) == 0)
      expo_help = 0;  // если экспонента 0
    //  не зайдём в следующий блок
    if (expo_help) {  // если эксопнента будет больше нуля по модулю
      if (nu[0] > '0') {  // положительная экспонента
        for (; nu[expo] != '.'; expo++)
          ;  // будет равна номеру позиции десятичной точки -1
      }      // единицу вычтем позже
      else {
        for (; nu[expo] == '0' || nu[expo] == '.'; expo++)
          ;
        // будет равна позиции первого ненулевого символа -1
      }        // вычтем позже
      expo--;  // вычитаем здесь
      if (nu[0] == '0')
        expo = -1 * expo;  // если экспонента должна быть отрицательной
    }
    if (*specificator == 'g' || *specificator == 'G') {
      if (*precision == 0) *precision = 1;
      if ((-4 <= expo && expo < *precision) ||
          !expo) {  // в этом случае g работает как f
        *precision = *precision - expo - 1;  // но меняет точность вот так
      } else {  // в этом случае работает как g/G
        *precision -= 1;  // но меняет точность вот так
        *specificator -= 2;  //  меняем спецификатор с g/G на e/E
        *og = 1;  // отмечаем, что изначально это был g/G
      }
    }
    if (*precision < 0) *precision = 0;  // если точност стала отрицательной
  }  // блок для обработки g/G закончился
}

/////// value_to_str process_g rounding_double cut_off_zeros  apply_sign
/// to_upper
int value_to_str(long double x, char *nu, long double x1, int *length) {
  int inf_or_nan = 0;
  if (isnan(x)) {           // если аргумент дал nan
    s21_strcat(nu, "nan");  // записываем его в строку
    inf_or_nan = 1;         // отмечаем его
  } else if (isinf(x)) {  // тоже самое с inf, но тут ещё есть знак
    if (isinf(x) > 0)
      s21_strcat(nu, "inf");
    else
      s21_strcat(nu, "-inf");
    inf_or_nan = 1;
  } else {  // если всё в порядке, записываем число в строку
    double_to_str(x, x1, nu, length);
  }
  return inf_or_nan;
}

void double_to_str(long double x, long double x1, char *nu, int *length) {
  if (x < 0) x = -1 * x;
  x1 = (x - fmodl(x, 1));
  char *kind_of_int = nu;

  if (x1 == 0)
    nu[0] = '0';
  else {
    recursive_long_int(x, 1, &kind_of_int);
  }
  s21_strcat(nu, ".");  // после печати целой части печатаем точку

  *length = s21_strlen(nu);  // отмечаем длину получившегося числа
  x1 = x =
      fabsl(fmodl(x, 1));  // берём дробную часть исходного числа (по модулю)
  char digit[2] = {'0', 0};  // будем использовать эту строку для стркатов
  for (int z = 0; z < 50;
       z++) {  // максимально печатаемая точность что-то около 49
    x1 = x = x * 10;  // первое числе после точки ставим перед точкой
    x1 -= fmodl(x1, 1);  // убираем дробную часть числа, так что останется
    // только цифра перед точкой
    digit[0] = x1 + '0';  // записываем эту цифру в строку для стркатов
    s21_strcat(
        nu,
        digit);  // добавляем её в строку, в которую записываем исходное число
    x1 = x = fmodl(x, 1);  // записываем сюда десятичную часть исходного числа
    // после того, как забрали из неё ещё один символ
  }
}

void recursive_long_int(long double d_arg, int symbols, char **str) {
  long double divider = powl(10, symbols);
  int temp =
      (int)(fmodl(d_arg, divider) / (divider / 10.));  // берём нужную цифру
  if (d_arg / divider >= 1) {  // если число можно делить дальше
    symbols++;
    recursive_long_int(d_arg, symbols, str);  // то делим дальше
  } else if ((int)d_arg == 0) {
    temp = 0;  // где нужно записать 0 - запишем 0
  }
  // my_itoa_symbol(temp, str);  // превратит цифру из инта в нужный чар
  **str = temp + '0';  // записываем в строчку нужный чар
  (*str)++;            // сдвигаемся
  **str = '\0';
}

void work_with_e(long double x, flags data, int precision, char **nu1, int og) {
  char *nu = *nu1;
  int i = 0;
  char *nunu = calloc(s21_strlen(nu), sizeof(char));
  e_to_str(x, nu, nunu, data, &i,
           precision);  // правильно записываем мантиссу в строку

  if (og && !data.sharp) {  // если g и нужно отрезать нули
    for (int s = s21_strlen(nu) - 1; s > 0 && nu[s] == '0'; s--) nu[s] = 0;
  }  // то отрезаем нули
  if (x != 0) {  // записываем значение экспоненты в строку
    if (i - 1 < 10) {  // если экспонента меньше 10
      s21_strcat(nunu, "0");  // первой цифрой экспоненты надо записать 0
      ourItoa(i - 1, &(nunu[3]),
              10);  // вставляем значение экспоненты после "e+0" или "e-0"
    } else {  // если экспонента больше 10 по модулю, 0 в начале не нужен
      ourItoa(i - 1, &(nunu[2]), 10);  // поэтому тут после "e+" или "e-"
    }
  }
  if (nu[s21_strlen(nu) - 1] == '.' && !data.sharp)
    nu[s21_strlen(nu) - 1] = 0;  // убираем точку, если она не нужна
  s21_strcat(nu, nunu);  // к строчке с мантиссой добавляем строчку, в которой
  // записали экспоненту
  free(nunu);
}

void e_to_str(long double x, char *nu, char *nunu, flags a, int *i,
              int precision) {
  if (nu[0] > '0') {  // в этом случае экспонента будет положительной
    for (; nu[(*i)] != '.'; (*i)++)
      ;  // и мы ищем её значение по положению точки
    for (int j = *i; j > 1; j--) nu[j] = nu[j - 1];  // сдвигаем число так
    nu[1] = '.';  // чтобы была одна цифра до десятичной точки, остальные после
    s21_strcat(nunu,
               "e+");  // строчка, в которую будем писать значение экспоненты
    rounding_double(2, precision, nu,
                    a);  // округляем мантиссу до нужной точности
    if (nu[-1] != 0) {  // если после округления число стало длинее
      nu[0] = '.';  // смещаем точку на один символ влево
      s21_memmove(
          &nu[1], &nu[2],
          precision + 1);  // смещаем оставшееся число к новому положению точки
      for (int y = precision + 1; y < 80; y++)
        nu[y] = 0;  // чистим ненужную часть строки
      (*i)++;  // инкрементируем значение экспоненты, потому что сдвинули точку
    }
  } else if (x == 0) {  // если исходное число равно нулю
    s21_strcat(nunu, "e+00");  // вот так будет выглядеть экспонента
    rounding_double(2, precision, nu, a);  // округляем число до нужной точности
  } else {  // иначе экспонента будет отрицательной
    for (; nu[(*i)] == '0' || nu[(*i)] == '.'; (*i)++)
      ;  // и равной положению первого ненулевой цифры
    nu[0] = nu[(*i)];  // первым символом станет первый ненулевой символ
    s21_memmove(&nu[2], &nu[(*i) + 1],
                precision + 1);  // смещаем к точке нужное количество символов
    rounding_double(2, precision, nu, a);  // округляем до нужной точности
    if (nu[-1] == '1') {  // если после округления число стало длиннее
      nu[0] = '.';  // смещаем точку влево
      s21_memmove(&nu[1], &nu[2],
                  precision + 1);  // смещаем оставшуюся часть числа к точке
      (*i)--;  // увеличиваем отрицательную экспоненту из-за смещения точки
    }
    // при увеличении отрицательная экспонента могла стать равной нулю
    if ((*i) - 1 == 0)
      s21_strcat(nunu, "e+");  // а с нулевой экспонентой надо писать "e+"
    else
      s21_strcat(nunu, "e-");  // иначе пишем "e-"
  }
}

void rounding_double(int length, int precision, char *nu, flags data) {
  // округляем десятичную дробь в соотвествии с нужной точностью
  unsigned int strlength = s21_strlen(nu);  // длина строки
  if ((unsigned int)(length + precision) <
      s21_strlen(nu)) {  // если строка должна быть короче
    int s = 1;
    int dot = 0;
    if (nu[length + precision] >= '5') {  // если предыдущую цифру нужно менять
      while (nu[length + precision - s] == '9' ||
             nu[length + precision - s] == '.') {
        // пока встречаем 9-ки или точку
        if (nu[length + precision - s] == '.')
          dot = 1;  // отмечаем точку
        else
          nu[length + precision - s] = '0';  // зануляем число
        s++;  // двигаемся дальше влево
      }
      // если мы оказались в позиции, где числа раньше не было
      if (nu[length + precision - s] == 0)
        nu[length + precision - s] = '1';  // печатаем туда единицу
      else
        nu[length + precision - s] +=
            1;  // иначе число станет больше на единицу
    }
    if (precision == 0)
      dot = 1;  // если нужна нулевая точность, тоже отметим точку
    if (!dot)
      nu[length + precision] =
          0;  // если точки не было, терминирующий 0 после числа
    else {  // если dot = 1, то цифры числа после точки надо отрезать при
      // округлении
      int dot_location = 0;
      for (; nu[dot_location] != '.'; dot_location++)
        ;  // находим точку
      if (!data.sharp && precision == 0)
        nu[dot_location] = 0;  // если её надо оставить - оставляем
      else
        nu[dot_location + precision + 1] =
            0;  // если нет - отрезаем вместе с остатком числа
    }
    for (unsigned int sl = s21_strlen(nu); sl < strlength; sl++)
      nu[sl] = 0;  // чистим остаток строки
  }
}

void cut_off_zeros(char *nu, flags data, int og) {  // обрезаем нули после точки
  if (og && !data.sharp) {  // если изначально это было g/G
    int there_is_dot = 0;   // todo
    for (unsigned int s = 0; s < s21_strlen(nu); s++)
      if (nu[s] == '.') there_is_dot = 1;
    for (int s = s21_strlen(nu) - 1; there_is_dot && s > 0 && nu[s] == '0'; s--)
      nu[s] = 0;
    // ищем точку и отрезаем нули в конце
  }
}

void to_upper(char *x) {
  // приводит буквы к верхнему регистру
  for (int i = 0; x[i]; i++) {
    if (x[i] >= 'a' && x[i] <= 'z') x[i] -= 32;
  }
}
