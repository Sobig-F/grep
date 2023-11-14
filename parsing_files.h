#ifndef PARSING_FILES
#define PARSING_FILES

#include "restring.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flags.h"
#include "find_struct.h"

char *get_new_str(regex_t *pattern, char *str) {
    char *new_str = (char*)malloc(sizeof(char) * (int)strlen(str));
    regmatch_t *pm = NULL;
    int len = (int)strlen(str);
    int size_new_str = 1;
    new_str[0] = *str;
    while (regexec(pattern, new_str, 0, pm, 0) && (size_new_str < len)) {
        new_str[size_new_str] = str[size_new_str];
        ++size_new_str;
    }
    free(new_str);
    new_str = (char*)malloc(sizeof(char) * (len - size_new_str + 1));
    for (int i = size_new_str; i < len; ++i) {
        new_str[i - size_new_str] = str[i];
    }
    return new_str;
}

char* print_cut_str(char *str, regex_t *pattern) {
    regmatch_t *pm = NULL;
    int s_fin_str = 1, s_start_str = 1, i;
    char *start_str = (char*)malloc(sizeof(char) * (int)strlen(str));
    char *fin_str = (char*)malloc(sizeof(char) * (int)strlen(str));
    start_str[0] = *str;
    for (int i = 1; regexec(pattern, start_str, 0, pm, 0); ++i) {
        ++s_start_str;
        start_str[s_start_str - 1] = *(str + i);
    }
    fin_str[0] = start_str[s_start_str - 1];
    while (regexec(pattern, fin_str, 0, pm, 0)) {
        ++s_fin_str;
        i = 0;
        for (int j = s_start_str - s_fin_str; j < s_start_str; ++j) {
            fin_str[i] = start_str[j];
            ++i;
        }
    }
    free(start_str);
    return fin_str;
}

int check_string(char *filename, char *str, regex_t **templates, struct flags *flags, int count_str) {
    int detect;
    regmatch_t *pm = NULL;
    char *new_str = NULL, *cut_str = NULL;
    for (int i = 0; (*(templates + i) != NULL); ++i) {
        detect = !regexec(*(templates + i), str, 0, pm, 0);
        if (flags->v) {
            detect = !detect;
        }
        if (detect && !flags->h && !flags->c && !flags->l) {
            printf("%s:", filename);
        }
        if (detect && flags->n) {
            printf("%d:", count_str);
        }
        if (detect && flags->o) {
            cut_str = print_cut_str(str, *(templates + i));
            printf("%s\n", cut_str);
            free(cut_str);
        } else if (detect && !flags->c && !flags->l) {
            printf("%s\n", str);
        }
        if (detect && ((int)strlen(str) > 0) && !flags->c && !flags->l && !flags->v && flags->o) {
            new_str = get_new_str(*(templates + i), str);
            check_string(filename, new_str, templates, flags, count_str);
            free(new_str);
        }
    }
    return detect;
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

void find_patterns_in_file(struct findStruct *grepStruct) {
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