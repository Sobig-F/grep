#ifndef PARSING_FILES
#define PARSING_FILES

#include "restring.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flags.h"
#include "find_struct.h"

void print_cut_str(char *str, regex_t *pattern) {
    regmatch_t *pm;
    char *start_str = (char*)malloc(sizeof(char));
    char *fin_str = (char*)malloc(sizeof(char));
    start_str[0] = *str;
    int s_fin_str = 1;
    int s_start_str = 1;
    int i;
    for (int i = 1; regexec(pattern, start_str, 0, pm, 0); ++i) {
        ++s_start_str;
        start_str = (char*)realloc(start_str, sizeof(char) * s_start_str);
        start_str[s_start_str - 1] = *(str + i);
    }
    fin_str[0] = start_str[s_start_str - 1];
    while (regexec(pattern, fin_str, 0, pm, 0)) {
        ++s_fin_str;
        fin_str = (char*)realloc(fin_str, sizeof(char) * s_start_str);
        i = 0;
        for (int j = s_start_str - s_fin_str; j < s_start_str; ++j) {
            fin_str[i] = start_str[j];
            ++i;
        }
    }
    printf("%s\n", fin_str);
}

int check_string(char *filename, char *str, regex_t **templates, struct flags *flags, int *detect_string, int count_str, int count_files) {
    int detect;
    regmatch_t *pm;
    int print_str = 2;

    for (int i = 0; (print_str > 1) && (*(templates + i) != NULL); ++i) {
        detect = !regexec(*(templates + i), str, 0, pm, 0);
        if (flags->v) {
            detect = !detect;
        }
        if (detect && print_str && flags->c) {
            *detect_string += 1;
            detect = 0;
            print_str = 1;
        }
        if (print_str && detect && (flags->l || ((count_files > 1) && !flags->h))) {
            printf("%s", filename);
            if (!flags->l) {
                printf(":");
            } else {
                print_str = 0;
            }
        }
        if (print_str && detect && flags->n && !flags->l) {
            printf("%d:", count_str);
        }
        if (print_str && detect && !flags->l) {
            if (!flags->o) {
                printf("%s", str);
            } else {
                print_cut_str(str, *(templates + i));
            }
        }
        if (detect && !flags->o) {
            print_str = 1;
        } else if (flags->o && !flags->l) {
            print_str = 2;
        }
    }

    return print_str;
}

void parsing_file(char *filename, FILE *file, regex_t **templates, struct flags *flags, int count_files, int *detect_count) {
    char *str = NULL;
    str = get_string(file);
    int count_str = 1, mozno = 1;
    while ((str != NULL) && mozno) {
        mozno = check_string(filename, str, templates, flags, detect_count, count_str, count_files);
        if (!flags->o && (*(str + strlen(str) - 1) != '\n')) {
            printf("-\n");
        }
        free(str);
        str = get_string(file);
        ++count_str;
    }
    free(str);
}

void find_patterns_in_file(struct findStruct *grepStruct) {
    FILE *file;
    char **no_such_file = NULL;
    int count_files = 0, detect_count = 0, no_such_file_count = 0;
    for (int i = 0; (count_files < 2) && (*(grepStruct->files + i) != NULL); ++i) {
        ++count_files;
    }
    for (int i = 0; *(grepStruct->files + i) != NULL; ++i) {
        file = fopen(*(grepStruct->files + i), "r");
        if (file != NULL) {
            parsing_file(*(grepStruct->files + i), file, grepStruct->templates, grepStruct->flags, count_files, &detect_count);
        } else if (!grepStruct->flags->s) {
            if (no_such_file == NULL) {
                no_such_file = (char**)malloc(sizeof(char*));
            } else {
                no_such_file = (char**)realloc(no_such_file, sizeof(char*) * no_such_file_count + 1);
            }
            no_such_file[no_such_file_count] = *(grepStruct->files + i);
            ++no_such_file_count;
        }
        fclose(file);
    }
    if (grepStruct->flags->c) {
        printf("%d", detect_count);
    }
    if (no_such_file != NULL) {
        for (int i = 0; i < no_such_file_count; ++i) {
            printf("grep: %s: No such file or directory\n", *(no_such_file + i));
        }
    }
}

#endif