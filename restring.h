#ifndef RESTRING_H
#define RESTRING_H

#include <stdlib.h>

char* get_string(FILE *file, int new_line) {
    char *str = NULL, buff[2];
    int i = 1;
    while ((buff[0] != '\n') && (fgets(buff, 2, file))) {
        if (str == NULL) {
            str = (char*)malloc(sizeof(char));
        }
        if ((buff[0] != '\n' && buff[0] != '\0') || new_line) {
            str = (char*)realloc(str, (int)sizeof(char) * i);
            str[i - 1] = buff[0];
            ++i;
        }
    }
    return str;
}

#endif