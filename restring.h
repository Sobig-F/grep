#ifndef RESTRING_H
#define RESTRING_H

#include <stdlib.h>

char *get_string(FILE *file, int new_line) {
  char buff[2] = {0};
  int i = 1;
  char *str = NULL;
  while ((buff[0] != '\n') && (fgets(buff, 2, file))) {
    if (str == NULL) {
      str = (char *)malloc(sizeof(char));
      // str[0] = '\n';
      str[0] = '\0';
    }
    if ((buff[0] != '\n' && buff[0] != '\0') || new_line || (i == 1)) {
      ++i;
      str = (char *)realloc(str, (int)sizeof(char) * i);
      str[i - 2] = buff[0];
      // str[i - 2] = '\n';
      str[i - 1] = '\0';
      // ++i;
      // str = (char*)realloc(str, (int)sizeof(char) * i);
      // str[i - 1] = '\0';
    }
    // if (new_line && str[i - 2] != '\n') {
    //     str[i - 1] = '\n';
    // }
  }
  if ((str != NULL) && (str[i - 2] != '\n') && new_line) {
    ++i;
    str = (char *)realloc(str, (int)sizeof(char) * i);
    str[i - 2] = '\n';
    // str[i - 2] = '\n';
    str[i - 1] = '\0';
  }
  return str;
}

#endif