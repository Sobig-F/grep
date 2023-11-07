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
    // if (findStruct->flags != NULL) {
    //     printf("c - %d\n", findStruct->flags->c);
    //     printf("e - %d\n", findStruct->flags->e);
    //     printf("f - %d\n", findStruct->flags->f);
    //     printf("h - %d\n", findStruct->flags->h);
    //     printf("i - %d\n", findStruct->flags->i);
    //     printf("l - %d\n", findStruct->flags->l);
    //     printf("n - %d\n", findStruct->flags->n);
    //     printf("o - %d\n", findStruct->flags->o);
    //     printf("s - %d\n", findStruct->flags->s);
    //     printf("v - %d\n", findStruct->flags->v);
    // }
    findStruct->files = find_file(argc, argv, findStruct->flags->e || findStruct->flags->f);
    // for (int i = 0; *(findStruct->files + i) != NULL; ++i) {
    //     printf("%s\n", *(findStruct->files + i));
    // }
    findStruct->templates = find_template(argc, argv, findStruct->flags);
    
    if (findStruct->flags->f && (findStruct->templates == NULL)) {
        clean_findStruct(findStruct);
        findStruct = NULL;
    }
    return findStruct;
}

#endif