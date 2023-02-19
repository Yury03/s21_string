#include "s21_string.h"
void *s21_memchr(const void *str, int c, s21_size_t n)
{
    unsigned char *buf = (unsigned char *)str;
    unsigned char *res = S21_NULL;
    // c ? c < 0 : -c;
   // c ? c > 127 : c % 127; todo
    bool find = false;
    s21_size_t i = 0;
    while (!find && i < n)
    {
        if (buf[i] == c)
        {
            res = &buf[i];
            find = true;
        }
        i++;
    }
    return res;
}