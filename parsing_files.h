#ifndef PARSING_FILES
#define PARSING_FILES

#include "restring.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flags.h"
#include "find_struct.h"
#include "sort.h"

void print_cut_str(char *str, int start, int end) {
    for (int i = start; i < end; ++i) {
        printf("%c", str[i]);
    }
    printf("\n");
}

regmatch_t** append_pm(regmatch_t** all_pm, regmatch_t* pm, int *len, int offset) {
    ++(*len);
    if (all_pm == NULL) {
        all_pm = (regmatch_t**)malloc(sizeof(regmatch_t*));
        // all_pm[*len - 1] = NULL;
    } else {
        all_pm = (regmatch_t**)realloc(all_pm, sizeof(regmatch_t*) * *len);
        // all_pm[*len - 1] = NULL;
    }
    // if (all_pm[*len - 1] == NULL) {
    all_pm[*len - 1] = (regmatch_t*)malloc(sizeof(regmatch_t));
    // }
    // if (all_pm[*len - 1] != NULL) {
    all_pm[*len - 1]->rm_so = pm->rm_so + offset;
    all_pm[*len - 1]->rm_eo = pm->rm_eo + offset;
    // }

    return all_pm;
}

regmatch_t** check_templates(regex_t *template, char *str, int *len, regmatch_t **all_pm) {
    int offset = 0;
    regmatch_t *pm = NULL;
    pm = (regmatch_t*)malloc(sizeof(regmatch_t));
    while (offset < (int)strlen(str)) {
        // printf("qqq");
        if (!regexec(template, str + offset, 1, pm, 0)) {
            all_pm = append_pm(all_pm, pm, len, offset);
            offset += (int)pm->rm_eo;
        } else {
            offset = (int)strlen(str);
        }
    }
    if (pm != NULL) {
        free(pm);
    }
    return all_pm;
}

int check_string(char *filename, char *str, regex_t **templates, struct flags *flags, int count_str, int *print_str) {
    int find_count = 0;
    regmatch_t **all_pm = NULL;
    for (int i = 0; *(templates + i) != NULL; ++i) {
        all_pm = check_templates(*(templates + i), str, &find_count, all_pm);
    }
    // free(all_pm);
    if (all_pm != NULL) {
        all_pm = (regmatch_t**)realloc(all_pm, sizeof(regmatch_t*) * (find_count + 1));
    }
    if (!flags->o && find_count) {
        find_count = 1;
    }
    if (find_count > 1) {
        quick_sort(all_pm, find_count);
    }
    if (flags->v && (find_count > 0)) {
        find_count = 0;
    } else if (flags->v && (find_count == 0)) {
        // printf("-----\n");
        find_count = 1;
    }

    for (int i = 0; (i < find_count) && (*print_str || !flags->o); ++i) {
        if (!flags->h && !flags->c && !flags->l) {
            printf("%s:", filename);
        }
        if (flags->n) {
            printf("%d:", count_str);
        }
        if (flags->o && !flags->v) {
            print_cut_str(str, (int)all_pm[i]->rm_so, (int)all_pm[i]->rm_eo);
        } else if (!flags->c && !flags->l) {
            printf("%s", str);
        }
    }
    if (all_pm != NULL) {
        for (int i = 0; *(all_pm + i) != NULL; ++i) {
            free(*(all_pm + i));
        }
        free(all_pm);
    }
    return find_count;
}

void parsing_file(char *filename, FILE *file, regex_t **templates, struct flags *flags, int *print_str) {
    char *str = NULL;
    str = get_string(file, 1);
    // printf("%s", str);
    int count_str = 1, detect = 0, detect_count = 0;
    while ((str != NULL) && (!detect || !flags->l)) {
        // printf("%s\n", str);
        detect = check_string(filename, str, templates, flags, count_str, print_str);
        if (detect) {
            detect_count += 1;
        }
        free(str);
        str = NULL;
        str = get_string(file, 1);
        // printf("%s", str);
        ++count_str;
    }
    if (flags->c && !flags->h) {
        printf("%s:", filename);
    }
    if (flags->c) {
        printf("%d\n", detect_count);
    }
    if (detect_count && flags->l) {
        printf("%s\n", filename);
    }
    if (str != NULL) {
        free(str);
    }
}

void parsing_all_filepath(struct findStruct *grepStruct) {
    FILE *file = NULL;
    // printf("IS OPEN");
    // if (grepStruct->files == NULL) {
    //     printf("files is NULL");
    // }
    // if (grepStruct->flags == NULL) {
    //     printf("flags is NULL");
    // }
    // if (grepStruct->templates == NULL) {
    //     printf("temp is NULL");
    // }
    if ((grepStruct->files != NULL) && (grepStruct->templates != NULL) && (grepStruct->flags != NULL)) {
        for (int i = 0; *(grepStruct->files + i) != NULL; ++i) {
            file = fopen(*(grepStruct->files + i), "r");
            if (file != NULL) {
                parsing_file(*(grepStruct->files + i), file, grepStruct->templates, grepStruct->flags, &grepStruct->print_str);
            }
            fclose(file);
        }
    }
}

#endif