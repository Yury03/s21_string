#include "s21_string.h"
int s21_memcmp(const void *str1, const void *str2, s21_size_t n)
{
    int res = 0;
    unsigned char *buf1 = (unsigned char *)str1;
    unsigned char *buf2 = (unsigned char *)str2;
    s21_size_t i = 0;
    if (buf1 != buf2)
    {
        while (buf1[i] == buf2[i] && i < n)
        {
            i++;
        }
        if (i != n)
        {
            res = buf1[i] - buf2[i];
        }
    }
    return res;
}