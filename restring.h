#ifndef RESTRING_H
#define RESTRING_H

#include <stdlib.h>

char* get_string(FILE *file) {
    char *str = NULL, buff[2];
    int i = 1;
    while ((buff[0] != '\n') && (fgets(buff, 2, file))) {
        if (str == NULL) {
            str = (char*)malloc(sizeof(char));
        }
        str = (char*)realloc(str, sizeof(char) * i);
        str[i - 1] = buff[0];
        ++i;
    }
    return str;
}

#endif