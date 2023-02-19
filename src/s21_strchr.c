#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
    char *pr = (char *)str;
    if (*pr == '\0' && c == 0) {
        return (pr); // УНИЧТОЖИТЬ!!!
    }
    for (; *pr != '\0'; pr++) {
        if (*pr == c) {
            return (char *)pr;//УНИЧТОЖИТЬ!!!
        }
    }
    if (*pr == '\0' && c == 0) {
        return (pr); // УНИЧТОЖИТЬ!!!
    }
    return 0; // УНИЧТОЖИТЬ!!!
}