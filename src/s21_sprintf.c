#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  flags data = {0};  // инициализируем все флаги нулями
  unsigned int count = 0;  // тут будем считать количество напечатанных символов
  va_list ptr;
  va_start(
      ptr,
      format);  // эти две строки - подготовка к работе с вариадической функцией
  while (format && *format) {  // пока форматированная строка не закончилась
    if (*format != '%') {  // если не наткнулись на символ процента
      *str = *format;
      str++;
      format++;
      count++;
    } else {        // если наткнулись на процент
      *str = '\0';  // зануляем текущий символ новой строки, чтобы можно было
      // делать стркат ДИМА РАЗБЕРИСЬ ПЖПЖПЖПЖПЖ Я НЕ ПОНИМАЮ
      // ЗАЧЕМ ЭТО
      format++;
      format = firstParser(
          format, &data);  // определяем все параметры, кроме спецификатора
      format = secondParser(format, &data);  // определяем спецификатор
      // в случае ошибки функции занулят форматированную строку
      if (format) {  // поэтому тут проверяем, что она ещё существует
        if (data.specificator == '%') {  // если спецификатор %
          *str = '%';
          str++;
          count++;  // то печатаем его и двигаемся дальше
        } else {  // иначе достаём аргумент и правильно печатаем его в строку
          str = callSpecifFun(&ptr, data, &str, &format, &count);
        }
      }
      cleanFlags(&data);  // чистим флаги перед работой со следующим аргументом
    }
  }
  *str = '\0';  // зануляем строку
  if (!format)
    count = -1;  // если строка была занулена, ставим возвращаемое значение для
  // ошибки
  va_end(ptr);  // закончили работу с вариадической функцией
  return count;
}

const char *firstParser(const char *str, flags *data) {
  while (*str == '-' || *str == '+' || *str == '0' || *str == '#' ||
         *str == ' ') {  // если есть флаг
    switch (*str) {
      case '-':
        data->minus = 1;
        break;
      case '+':
        data->plus = 1;
        break;
      case '#':
        data->sharp = 1;
        break;
      case '0':
        data->zero = 1;
        break;
      default:
        data->space = 1;
        break;
    }
    str++;
  }

  if (*str == '*') {  // если ширина равна звёздочке
    str++;  // отмечаем это на будущее и двигаемся дальше
    data->width = -2;
  } else {
    // data->width = stringToInteger(str);
    // то запишем это число в соответствующий элемент структуры
    while (*str >= '0' && *str <= '9') {  // если ширина есть и задана числом
      data->width *= 10;  // цикл нужен, потому что ширина может быть задана
                          // многосимвольным числом
      data->width += *str - '0';
      str++;
    }
  }

  if (*str == '.') {  // если есть символ точки, значит задана точность
    str++;
    data->prec = 1;  // отмечаем это
    if (*str == '*') {  // если точность задана звёздочкой
      data->precision = -2;
      str++;  // отмечаем это на будущее и идём дальше
    } else {
      // data->precision = stringToInteger(str);

      while (*str >= '0' && *str <= '9') {  // парсим аналогично ширине
        data->precision *= 10;
        data->precision += *str - '0';
        str++;
      }
    }
  }
  if (*str == 'h' || *str == 'l' || *str == 'L') {  // если задана длина
    data->length = *str;
    str++;  // записываем её и двигаемся дальше
  }
  return str;  // возвращаем элемент форматированной строки, который стоит после
               // флагов
}
const char *secondParser(const char *str, flags *data) {
  int find = 1;  // если ничего не найдено то будет код ошибки

  // if (*str >= 48 && *str <= 57) { нужно протестить с ним или без него
  //   data->width = stringToInteger(str); так как с ним chars
  //   str++; работает нормально
  // }

  if (!(*str >= 48 || *str <= 57)) {
    switch (*str) {
      case 'd':
      case 'i':
      case 'o':
      case 'u':
      case 'x':
      case 'X':
      case 'e':
      case 'E':
      case 'f':
      case 'g':
      case 'G':
      case 'c':
      case 's':
      case 'p':
      case 'n':
        break;
      default:
        find = 0;
        break;
    }
  }

  data->specificator = *str;
  str++;  // имеет смысл только если спецификатор существует
  if (!find) {
    str = S21_NULL;  // если ни один спецификатор не подошёл
  }
  return str;  // зануляем строку, чтобы отметить ошибку и возвращаем её
}
char *callSpecifFun(va_list *ptr, flags data, char **str, const char **format,
                    unsigned int *count) {
  if (data.width == -2) {  // если ширина была задана звёздочкой
    int width =
        va_arg(*ptr, size_t);  // достаём аргумент, в котором записана ширина
    data.width = width;  // и приравниваем ширину к его значению
  }
  if (data.precision == -2) {  // аналогично ширине
    int ac = va_arg(*ptr, size_t);
    data.precision = ac;
  }
  switch (data.specificator)  // в зависимости от типа спецификатора, переходим
                              // в нужную функцию
  {
    case 'd':
    case 'i':
      *str = integers(str, data, ptr, count);
      break;
    case 'o':
    case 'u':
    case 'x':
    case 'X':
      *str = uintegers(str, data, ptr, count);
      break;
    case 'e':
    case 'E':
    case 'f':
    case 'g':
    case 'G':
      *str = doubles(str, data, ptr, count);
      break;
    case 'c':
      *str = chars(str, data, ptr, format, count);
      break;
    case 's':
      *str = charPointers(str, data, ptr, format, count);
      break;
    case 'p':
      *str = pointers(str, data, ptr, count);
      break;
    case 'n':
      *str = integerPointers(str, ptr, *count);
      break;
    default:
      break;
  }
  return *str;  // вернём элемент строки, с которого надо продолжить печать
}

char *integers(char **str, flags data, va_list *ptr, unsigned int *count) {
  long int x = va_arg(
      *ptr, long int);  // достаём аргумент, который надо будет напечатать
  int i = 0;
  int size;
  char *array = calloc(21, sizeof(char));
  char *tmp = array;
  if (data.length == 'h') {
    x = (short)x;
  } else if (data.length == 0) {
    x = (int)x;  // меняем тип данных в зависимости от параметра длины
  }
  ourItoa(x, array, 10);  // переводим число в строку
  if (x == 0 && data.prec && !data.precision) {
    array[0] = 0;  // особенность работы с нулём
  }
  if (array[0] == '-') array++;  // пока не берём во внимание минус

  if (21 > (data.precision > data.width ? data.precision : data.width)) {
    size = 21 + 2;
  } else if (data.precision > data.width) {
    size = data.precision + 2;
  } else {
    size = data.width + 2;
  }
  char *result = calloc(size, sizeof(char));  // аллоцируем память для записи
                                              // числа с нужным форматированием
  widthHelper(result, data, &i, s21_strlen(array));  // обрабатываем ширину
  if (x < 0) {
    negativeSign(result, i);  // ставим знак, если нужен
  } else {
    positiveSign(data, result, i);
  }
  for (; (data.precision - (int)s21_strlen(array)) > 0; i++, data.precision--) {
    s21_strcat(result, "0");  // дополняем до нужной точности нулями
  }

  s21_strcat(result, array);  // вставляем число в массив, где сделали
                              // правильное форматирование
  if (data.minus) {  // если есть флаг минус
    while ((int)s21_strlen(result) < data.width) s21_strcat(result, " ");
  }  // то пробелы для ширины надо вставлять уже после числа
  s21_strcat(*str, result);
  unsigned int check =
      s21_strlen(result);  // считаем, сколько символов добавится
  *count += check;  // учитываем их в подсчёте напечатанных символов
  free(result);
  free(tmp);
  return *str + check;
}
char *uintegers(char **str, flags data, va_list *ptr, unsigned int *count) {
  unsigned long int x = va_arg(*ptr, unsigned long int);
  int size;
  char num[24] = {0};  // массив для числа
  if (data.length == 'h')
    x = (unsigned short)x;  // в зависимости от длины
  else if (data.length == 0)
    x = (unsigned int)x;  // приводим к нужному типу данных
  if (data.specificator == 'o')
    ourUItoa(x, num, 8);  // 8-чный
  else if (data.specificator == 'u')
    ourUItoa(x, num, 10);  // 10-чный
  else
    ourUItoa(x, num, 16);  // 16-чный
  if (data.specificator == 'X')
    to_upper(num);  // в этом флаге x в базе печатается апперкейсом
  if (x == 0 && data.prec && !data.precision)
    num[0] = 0;  // так оригинал воспринимает 0
  int i = 0;     // j = 0;
  if (24 > (data.precision > data.width ? data.precision : data.width)) {
    size = 24 + 2;
  } else if (data.precision > data.width) {
    size = data.precision + 2;
  } else {
    size = data.width + 2;
  }
  char *result =
      calloc(size, sizeof(char));  // массив для форматированного числа
  widthHelper(result, data, &i, s21_strlen(num));  // печатаем ширину
  print_base(data, x, result, &i, &(data.precision));  // печатаем базу

  for (; data.precision - (int)s21_strlen(num) > 0; i++, data.precision--) {
    result[i] = '0';  // если нужна точность - печатаем её
  }
  result[i] = '\0';
  s21_strcat(result, num);  // добавляем число в форматированную строку
  if (data.minus) {  // если ширину надо печатать после числа
    while ((int)s21_strlen(result) < data.width) {
      s21_strcat(result, " ");  // печатаем
    }
  }
  s21_strcat(*str,
             result);  // добавляем форматированное число в финальную строку
  unsigned int add = s21_strlen(result);  // считаем новые символы
  *count += s21_strlen(result);  // увеличиваем счётчик напечатанных символов
  free(result);
  return *str + add;  // сдвигаемся по финальной строке
}

char *doubles(char **str, flags data, va_list *ptr, unsigned int *count) {
  long double x, x1;
  int og = 0, i = 0, length = 0, inf_or_nan = 0;
  if (data.length == 'L')
    x1 = x =
        va_arg(*ptr, long double);  // выбираем нужный тип данных для аргумента
  else
    x1 = x = va_arg(*ptr, double);  // в соответствии с длиной
  char num[500] = {0};  // массив, в который будем писать число
  char *nu = &(num[1]);  // оставляем первую ячейку на случай удлиннения числа
  int precision = data.prec ? data.precision : 6;
  inf_or_nan = value_to_str(x, nu, x1, &length);  //  запишем число в строку
  if (!inf_or_nan) {  // если нормальное число
    process_g(&(data.specificator), &precision, nu, &og,
              x);  // предобработка для g/G
    if (data.specificator == 'e' || data.specificator == 'E') {  // если e/E
      work_with_e(x, data, precision, &nu,
                  og);  // обработаем и округлим как e/E
    } else {            // иначе
      rounding_double(length, precision, nu, data);  // как f
      cut_off_zeros(nu, data,
                    og);  // если нужно, отрежем нули после десятичной точки
    }
    if ((nu[s21_strlen(nu) - 1] == '.') &&
        !(data.specificator == 'f' && data.sharp))
      nu[s21_strlen(nu) - 1] = 0;
    // если последним символом стала точка и её не нужно оставлять - убираем
  }
  if (nu[-1]) nu = num;
  int size = ((unsigned int)(data.width + 2) > (s21_strlen(nu) + 2)
                  ? (unsigned int)(data.width + 2)
                  : (s21_strlen(nu) + 2));
  char *result =
      calloc(size + 1, sizeof(char));  // массив для отформатированного числа
  for (; !data.minus && (data.width - (int)s21_strlen(nu) > 0);
       i++, data.width--) {
    if (data.zero)
      result[i] = '0';  // заполняем ширину нулями
    else
      result[i] = ' ';  // или пробелами
  }
  if (!inf_or_nan)
    if (x < 0.) {
      negativeSign(result, i);
    } else {
      positiveSign(data, result, i);
    }
  else if (isinf(x) > 0 || isnan(x)) {
    positiveSign(data, result, i);
  }
  if (data.specificator == 'E' || data.specificator == 'G')
    to_upper(nu);  // если нужен апперкейс
  if (num[0] != 0)
    s21_strcat(result,
               num);  // копируем в финальную строку вместе с нулевой ячейкой
  else
    s21_strcat(result,
               nu);  // или без; смотря, появилась ли там цифра после округления
  if (data.minus)
    for (; (int)s21_strlen(result) < data.width;)
      s21_strcat(result, " ");  // ширина
  int add = s21_strlen(result);
  s21_strcat(*str, result);
  *count += add;
  free(result);
  return *str + add;
}

char *chars(char **str, flags data, va_list *ptr, const char **format,
            unsigned int *count) {
  // берём аргумент
  wchar_t symbol = va_arg(*ptr, wchar_t);

  // с правильным типом данных в зависимости от длины
  if (data.length != 'l') {
    symbol = (char)symbol;
  }

  // если у нас широкий символ, то зануляем форматированную строку
  if (data.length == 'l' && symbol > 127) {
    (**str) = '\0';
    *format = 0;
  } else {
    // если нужно допечатать ширину пробелами
    for (; !data.minus && (data.width - 1 > 0);
         (*str)++, data.width--, (*count)++) {
      (**str) = ' ';
    }

    // пишем в строчку нужный символ
    (**str) = symbol;
    (*str)++, (*count)++;  // сдвигаемся по строчке и учитываем напечатанный
                           // символ в подсчёте

    // если есть флаг минус, то делаем ширину уже после символа
    if (data.minus) {
      for (; (data.width - 1 > 0); (*str)++, data.width--, (*count)++) {
        (**str) = ' ';
      }
    }

    // ставим нулевой символ в конец строки
    **str = '\0';
  }

  return *str;
}

char *charPointers(char **str, flags data, va_list *ptr, const char **format,
                   unsigned int *count) {
  if (data.length ==
      'l') {  // обрабатываем строку широких символов, если необходимо
    wCharPointers(str, data, ptr, format, count);
  } else {  // обрабатываем обычную строку символов
    char *x = va_arg(*ptr, char *);  // получаем указатель на символ
    int length = x == NULL ? 6 : s21_strlen(x);  // получаем длину строки
    if (data.prec && data.precision < length)
      length = data.precision;  // применяем точность, если указана
    if (data.prec && x == NULL && data.precision < 6)
      length = 0;  // обрабатываем случай с пустым указателем и точностью
    int size = (length > data.width ? length : data.width) +
               2;  // вычисляем размер результирующей строки
    char *result = calloc(
        size, sizeof(char));  // выделяем память под результирующую строку
    int i = 0;
    // заполняем ширину пробелами, если необходимо
    for (; !data.minus && (data.width - length > 0); i++, data.width--) {
      if (data.zero)
        result[i] = '0';
      else
        result[i] = ' ';
    }

    // копируем строку в результирующую строку
    if (x != NULL) {
      for (int q = 0; q < length; q++, i++) {
        result[i] = x[q];
      }
    } else if (length != 0) {
      char *string = "(null)";
      for (int rt = 0; rt < 7; rt++, i++) result[i] = string[rt];
    }
    result[i] = '\0';
    // заполняем ширину пробелами, если необходимо (для флага minus)
    if (data.minus) {
      for (; s21_strlen(result) < (s21_size_t)data.width; i++) {
        result[i] = ' ';
      }
    }
    s21_strcat(*str,
               result);  // добавляем результирующую строку к выходной строке
    *str += s21_strlen(result);  // обновляем указатель на выходную строку
    *count += s21_strlen(result);  // обновляем количество символов
    free(result);  // освобождаем память, используемую для результирующей строки
  }
  return *str;  // возвращаем указатель на выходную строку
}

char *wCharPointers(char **str, flags data,  // для работы с символами,
                    va_list *ptr,  // которые выходят за рамки ASCII
                    const char **format, unsigned int *count) {
  // почти то же самое, что для строки обычных символов
  int asciiSymbols = 0;
  wchar_t *symbols = va_arg(*ptr, wchar_t *);
  if (symbols != 0) {  // но если строка ненулевая
    for (; asciiSymbols >= 0 && symbols[asciiSymbols]; asciiSymbols++)
      if (symbols[asciiSymbols] > 127) asciiSymbols = -2;
    // и там правда есть широкий символ, выходящий за рамки ASCII
  }

  if (asciiSymbols < 0) {
    // то функция поставит терминирующий 0 в финальную строку и вернёт
    // ошибку
    **str = 0;
    *format = 0;  // так мы отмечаем, что случилась ошибка
  } else {  // дальше как в обычной строке
    int i = 0;
    int length = symbols == 0 ? 6 : asciiSymbols;
    if (data.prec && data.precision < length) length = data.precision;
    if (data.prec && symbols == 0 && data.precision < 6) length = 0;
    int size = (length > data.width ? length : data.width) + 2;
    char *result = calloc(size, sizeof(char));

    for (; !data.minus && (data.width - length > 0); i++, data.width--) {
      result[i] = ' ';
    }
    if (symbols != 0) {
      for (int q = 0; q < length; q++, i++) {
        result[i] = symbols[q];
      }
    } else if (length != 0) {
      char *string = "(null)";
      for (int rt = 0; rt < 7; rt++, i++) result[i] = string[rt];
    }
    result[i] = '\0';
    if (data.minus) {
      for (; (int)s21_strlen(result) < data.width;) s21_strcat(result, " ");
    }
    s21_strcat(*str, result);
    *str += s21_strlen(result);
    *count += s21_strlen(result);
    free(result);
  }
  return *str;
}

char *pointers(char **str, flags data, va_list *ptr, unsigned int *count) {
  long long int A = va_arg(*ptr, long long int);
  char adress[20] = {0};  // здесь хранится значение поинтера без форматирования

  if (A == 0) {
    s21_strcat(adress, "(null)");
  } else {
    ourUItoa(A, adress, 16);  // записываем значение поинтера
  }

  int size = 0;

  if ((20 + 4) > (data.precision + 4 > data.width + 2 ? data.precision + 4
                                                      : data.width + 2)) {
    size = 20 + 4;
  } else if ((data.precision + 4) > (data.width + 2)) {
    size = data.precision + 4;
  } else {
    size = data.width + 2;
  }

  // массив для записи с форматированием
  // 0x в начале числа учитывается в ширине, но не в точности, поэтому в
  // одном месте +2, в другом +4
  char *result = calloc(size, sizeof(char));
  int i = 0;

  for (; !data.minus &&
         (data.width -
              ((data.precision * (A != 0)) > (int)s21_strlen(adress)
                   ? (data.precision * (A != 0))
                   : (int)s21_strlen(adress)) -
              2 * (A != 0) >
          0);
       i++, data.width--) {
    result[i] = ' ';  // если нужно печатать ширину - печатаем
  }

  if (A != 0) {  // если не нулевой указатель
    if (data.plus == 1 || data.space == 1) {  // и нужно напечатать + или пробел
      if (i != 0)
        result[i - 1] = data.plus ? '+' : ' ';  // печатаем за счёт ширины
      else {  // а если ширины в начале не было
        result[i] = data.plus ? '+' : ' ';  // печатаем первым символом
        i++;  // и сдвигаемся по строке
      }
    }
    result[i] = '0';  // печатаем базу 16-ричного числа
    result[i + 1] = 'x';  // тут тоже
    i += 2;  // и сдвигаеся для дальнейшей печати на 2 символа
  }

  for (; A != 0 && data.precision - (int)s21_strlen(adress) > 0;
       i++, data.precision--) {
    result[i] = '0';  // если нужно дополнить нулями для точности - делаем это
  }

  result[i] = '\0';
  // записываем значение поинтера в форматированную строчку
  s21_strcat(result, adress);

  if (data.minus) {  // если шириной нужно дополнять после поинтера
    for (; (int)s21_strlen(result) < data.width;) s21_strcat(result, " ");
  }  // дополняем

  unsigned int add = s21_strlen(result);  // сколько символов напечатали

  // копируем форматрованную строчку в финальную строчку
  s21_strcat(*str, result);
  *count += add;  // увеличиваем счётчик напечатанных символов
  free(result);
  return *str + add;  // сдвигаемся по финальной строке для дальнейшей печати
}

char *integerPointers(char **str, va_list *ptr, unsigned int count) {
  int *x = va_arg(*ptr, int *);  // этот флаг ничего не дописывает в строку
  *x = count;  // берёт место, в которое записать инт
  return *str;  // записывает количество напечатанных к данному моменту
                // символов
}
