#include "s21_sprintf.h"
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
    data->width = stringToInteger(str);
    // то запишем это число в соответствующий элемент структуры
  }

  if (*str == '.') {  // если есть символ точки, значит задана точность
    str++;
    data->prec = 1;  // отмечаем это
    if (*str == '*') {  // если точность задана звёздочкой
      data->precision = -2;
      str++;  // отмечаем это на будущее и идём дальше
    } else {
      data->precision = stringToInteger(str);
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
  switch (*str) {
    case 'd':
    case 'i':
      // works with int
      break;
    case 'o':
    case 'u':
    case 'x':
    case 'X':
      // works with uints
      break;
    case 'e':
    case 'E':
    case 'f':
    case 'g':
    case 'G':
      // works with doubles
      break;
    case 'c':
      // work with char
      break;
    case 's':
      // work with string or char pointer
      break;
    case 'p':
      // work with pointer
      break;
    case 'n':
      // work with intpointer
      break;
    default:
      find = 0;
      break;
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
    while (s21_strlen(result) < data.width) s21_strcat(result, " ");
  }  // то пробелы для ширины надо вставлять уже после числа
  s21_strcat(*str, result);
  unsigned int check =
      s21_strlen(result);  // считаем, сколько символов добавится
  *count += check;  // учитываем их в подсчёте напечатанных символов
  free(result);
  return *str + check;
}
char *uintegers(char **str, flags data, va_list *ptr, unsigned int *count) {}

char *doubles(char **str, flags data, va_list *ptr, unsigned int *count) {}

char *chars(char **str, flags data, va_list *ptr, const char **format,
            unsigned int *count) {
  int symbol = va_arg(*ptr, int);
  if (data.length != 'l')
    symbol = (char)symbol;  // берем аргумент с нужным типом данных

  if (data.length != 'l' && symbol < 127) {  // если символ из аски
    while (!data.minus && data.width - 1 > 0) {  // печатаем ширину пробелами
      **str = ' ';
      (*str)++;
      data.width--;
      (*count)++;
    }

    (**str) = symbol;  // пишем в строчку новый символ
    (*str)++;
    (*count)++;  // сдувигаемся по строчке и учитываем напечтанный символ в
                 // подсчете

    if (data.minus) {  // если флаг минус, то делаем то же самое
      while (data.width - 1 > 0) {
        **str = ' ';
        (*str)++;
        data.width--;
        (*count)++;
      }
      **str = '\0';
    }
  } else {
    **str = '\0';  // так делает ориг спринтф
    *format = 0;
  }
  return *str;
}

char *charPointers(char **str, flags data, va_list *ptr, const char **format,
                   unsigned int *count) {
  if (data.specificator == 'l') {
    wCharPointers(str, data, ptr, format,
                  count);  // для работы с широкими символами wchar
  } else {
    int i = 0;
    char *symbols = va_arg(*ptr, char *);  // берем поинтер на символ
    int length = 0;

    if (symbols == 0) {
      length = 6;
    } else {
      length = (int)s21_strlen(symbols);
    }

    if (data.prec && data.precision < length) length = data.precision;
    if (data.prec && symbols == 0 && data.precision < 6) length = 0;
    int size = (length > data.width ? length : data.width) + 2;
    char *result = calloc(
        size, sizeof(char));  // нужно будет зафришить. массив нужной длины
    for (; !data.minus && (data.width - length > 0); i++, data.width--) {
      result[i] = ' ';  // заполняем ширину
    }
    if (symbols != 0) {  // если указатель не нулевой
      s21_strcpy(result, symbols);  // копируем нужную строку
    } else if (length != 0) {  // если указатель нулевой и длины хватит
      char *null_string = "(null)";
      s21_strcpy(result, null_string);  // копируем строку
    }
    i = s21_strlen(result) - 1;
    result[i] = '\0';

    if (data.minus) {
      for (; (int)s21_strlen(result) < data.width;)
        s21_strcat(result, " ");  // дополняем ширину при необходимости
    }
    s21_strcat(*str, result);
    *str += s21_strlen(result);
    *count += s21_strlen(result);

    free(result);
  }

  return *str;
}

char *wCharPointers(char **str, flags data,  // для работы с символами,
                    va_list *ptr,  // которые выходят за рамки ASCII
                    const char **format, unsigned int *count) {
  int asciiSymbol = 0;
  wchar_t *symbols = va_arg(*ptr, wchar_t *);

  if (symbols != 0) {  // если строка ненулевая и широкий символ
    for (; asciiSymbol >= 0 && symbols[asciiSymbol]; asciiSymbol++)
      if (symbols[asciiSymbol] > 127) asciiSymbol = -2;
  }

  if (asciiSymbol < 0) {
    **str = 0;
    *format = 0;  // так мы отмечаем, что есть ошибка
  } else {        // дальше как в *charPointers
    int i = 0;
    char *symbols = va_arg(*ptr, char *);
    int length = 0;

    if (symbols == 0) {
      length = 6;
    } else {
      length = (int)s21_strlen(symbols);
    }

    if (data.prec && data.precision < length) length = data.precision;
    if (data.prec && symbols == 0 && data.precision < 6) length = 0;
    int size = (length > data.width ? length : data.width) + 2;
    char *result = calloc(size, sizeof(char));  // нужно будет зафришить
    for (; !data.minus && (data.width - length > 0); i++, data.width--) {
      result[i] = ' ';
    }
    if (symbols != 0) {
      s21_strcpy(result, symbols);
    } else if (length != 0) {
      char *null_string = "(null)";
      s21_strcpy(result, null_string);
    }
    i = s21_strlen(result) - 1;
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

char *pointers(char **str, flags data, va_list *ptr, unsigned int *count) {}

char *integerPointers(char **str, va_list *ptr, unsigned int count) {
  int *x = va_arg(*ptr, int *);  // этот флаг ничего не дописывает в строку
  *x = count;  // берёт место, в которое записать инт
  return *str;  // записывает количество напечатанных к данному моменту символов
}
