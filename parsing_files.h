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
    } else {
        all_pm = (regmatch_t**)realloc(all_pm, sizeof(regmatch_t*) * *len);
    }
    all_pm[*len - 1] = (regmatch_t*)malloc(sizeof(regmatch_t));
    all_pm[*len - 1]->rm_so = pm->rm_so + offset;
    all_pm[*len - 1]->rm_eo = pm->rm_eo + offset;

    return all_pm;
}

regmatch_t** check_templates(regex_t *template, char *str, int *len, regmatch_t **all_pm, int *temp_count) {
    int offset = 0, count = 0;
    regmatch_t *pm = NULL;
    pm = (regmatch_t*)malloc(sizeof(regmatch_t));
    while (offset < (int)strlen(str)) {
        if (regexec(template, str + offset, 1, pm, 0) == 0) {
            all_pm = append_pm(all_pm, pm, len, offset);
            ++count;
            offset += (int)pm->rm_eo;
        } else {
            offset = (int)strlen(str);
        }
    }
    if (count > 0) {
        ++(*temp_count);
    }
    if (pm != NULL) {
        free(pm);
    }
    return all_pm;
}

int check_string(char *filename, char *str, regex_t **templates, struct flags *flags, int count_str) {
    int find_count = 0, temp_count = 0;
    regmatch_t **all_pm = NULL;
    for (int i = 0; *(templates + i) != NULL; ++i) {
        all_pm = check_templates(*(templates + i), str, &find_count, all_pm, &temp_count);
    }
    free(all_pm);
    if (find_count > 1) {
        quick_sort(all_pm, find_count);
    }
    if (!flags->o) {
        find_count = temp_count;
    }
    if (flags->v && (find_count > 0)) {
        find_count = 0;
    } else if (flags->v && (find_count == 0)) {
        find_count = 1;
    }

    for (int i = 0; i < find_count; ++i) {
        if (!flags->h && !flags->c && !flags->l) {
            printf("%s:", filename);
        }
        if (flags->n) {
            printf("%d:", count_str);
        }
        if (flags->o && !flags->v) {
            print_cut_str(str, (int)all_pm[i]->rm_so, (int)all_pm[i]->rm_eo);
        } else if (!flags->c && !flags->l) {
            printf("%s\n", str);
        }
    }
    if (all_pm != NULL) {
        for (int i = 0; all_pm[i] != NULL; ++i) {
            free(*(all_pm + i));
        }
        free(all_pm);
    }

    return find_count;
}

void parsing_file(char *filename, FILE *file, regex_t **templates, struct flags *flags) {
    char *str = NULL;
    str = get_string(file, 0, str);
    int count_str = 1, detect = 0, detect_count = 0;
    while ((str != NULL) && (!detect || !flags->l)) {
        detect = check_string(filename, str, templates, flags, count_str);
        free(str);
        str = NULL;
        if (detect) {
            detect_count += 1;
        }
        str = get_string(file, 0, str);
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
    FILE *file;
    int count_files = 0;
    for (int i = 0; (count_files < 2) && (*(grepStruct->files + i) != NULL); ++i) {
        ++count_files;
    }
    for (int i = 0; *(grepStruct->files + i) != NULL; ++i) {
        file = fopen(*(grepStruct->files + i), "r");
        if (file != NULL) {
            parsing_file(*(grepStruct->files + i), file, grepStruct->templates, grepStruct->flags);
        }
        fclose(file);
    }
}

#endif