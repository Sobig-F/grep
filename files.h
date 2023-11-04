#ifndef FILES_H
#define FILES_H

#include "flags.h"
#include "templates.h"

char** find_file(int argc, char *argv[], int s) {
    char **paths = NULL;
    int pathSize = 0;
    for (int i = 1; i < argc; ++i) {
        if ((((argv[i - 1][1] != 'f') && (argv[i - 1][1] != 'e')) || (argv[i - 1][0] != '-')) && (argv[i][0] != '-')) {
            ++pathSize;
            if (paths == NULL) {
                paths = (char**)malloc(sizeof(char*));
            } else {
                paths = (char**)realloc(paths, sizeof(char*) * pathSize);
            }
            paths[pathSize - 1] = (char*)malloc(sizeof(char) * (int)strlen(argv[i]));
            copy(paths[pathSize - 1], argv[i]);
        }
    }
    ++pathSize;
    paths = (char**)realloc(paths, sizeof(char*) * pathSize);
    return paths;
}

#endif