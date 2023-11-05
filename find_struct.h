#ifndef FIND_STRUCT_H
#define FIND_STRUCT_H

#include "files.h"
#include "flags.h"

struct findStruct {
    struct flags *flags;
    char **files;
    regex_t **templates;
};

void clean_findStruct(struct findStruct *findStruct) {
    if (findStruct->flags != NULL) {
        free(findStruct->flags);
    }
    if (findStruct->templates != NULL) {
        for (int i = 0; *(findStruct->templates + i) != NULL; ++i) {
            free(*(findStruct->templates + i));
        }
        free(findStruct->templates);
    }
    if (findStruct->files != NULL) {
        for (int i = 0; *(findStruct->files + i) != NULL; ++i) {
            free(*(findStruct->files + i));
        }
        free(findStruct->files);
    }
    free(findStruct);
}

struct findStruct* Create_find_struct(int argc, char * argv[]) {
    struct findStruct *findStruct = (struct findStruct*)malloc(sizeof(struct findStruct));
    findStruct->flags = flagging(argc, argv);
    findStruct->files = find_file(argc, argv, findStruct->flags->e || findStruct->flags->f);
    findStruct->templates = find_template(argc, argv, findStruct->flags);
    if (findStruct->flags->f && (findStruct->templates == NULL)) {
        clean_findStruct(findStruct);
        findStruct = NULL;
    }
    return findStruct;
}

#endif