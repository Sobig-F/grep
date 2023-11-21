#ifndef FIND_STRUCT_H
#define FIND_STRUCT_H

#include "files.h"
#include "flags.h"

struct findStruct {
    struct flags *flags;
    char **files;
    regex_t **templates;
    int print_str;
};

void clean_findStruct(struct findStruct *findStruct) {
    if (findStruct->flags != NULL) {
        free(findStruct->flags);
    }
    if (findStruct->templates != NULL) {
        // int i = 0;
        for (int i = 0; *(findStruct->templates + i) != NULL; ++i) {
            regfree(*(findStruct->templates + i));
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

struct findStruct* Create_find_struct(int argc, char * argv[], struct findStruct* findStruct) {
    if (findStruct != NULL) {
        findStruct->flags = flagging(argv);
        // printf("i - %d\n", findStruct->flags->i);
        // printf("v - %d\n", findStruct->flags->v);
        // printf("c - %d\n", findStruct->flags->c);
        // printf("l - %d\n", findStruct->flags->l);
        // printf("n - %d\n", findStruct->flags->n);
        // printf("h - %d\n", findStruct->flags->h);
        // printf("s - %d\n", findStruct->flags->s);
        // printf("o - %d\n", findStruct->flags->o);
        // printf("f - %d\n", findStruct->flags->f);
        // printf("e - %d\n", findStruct->flags->e);
        if (findStruct->flags != NULL) {
            findStruct->files = find_file(argc, argv, findStruct->flags->e || findStruct->flags->f);
            // for (int i = 0; *(findStruct->files + i) != NULL; ++i) {
            //     printf("%s\n", *(findStruct->files + i));
            // }
        }
        if (findStruct->flags != NULL && argv != NULL) {
            findStruct->print_str = 1;
            findStruct->templates = find_template(argv, findStruct->flags, &findStruct->print_str);
        }
    }

    return findStruct;
}

struct findStruct* Malloced_find_struct(int argc, char * argv[]) {
    struct findStruct *findStruct = NULL;
    if (findStruct == NULL) {
        findStruct = (struct findStruct*)malloc(sizeof(struct findStruct));
    }
    if (findStruct != NULL && argv != NULL) {
        findStruct->files = NULL;
        findStruct->flags = NULL;
        findStruct->templates = NULL;
        findStruct = Create_find_struct(argc, argv, findStruct);
    }
    if ((findStruct->files == NULL) || (findStruct->flags == NULL) || (findStruct->templates == NULL)) {
        clean_findStruct(findStruct);
        findStruct = NULL;
    }
    return findStruct;
}

#endif