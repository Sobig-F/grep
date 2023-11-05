#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "flags.h"

void clean_mass(char **arr) {
    for (int i = 0; arr[i] != NULL; ++i) {
        free(arr[i]);
    }
    free(arr);
}

void copy(char *first, char *second) {
    for (int i = 0; i < (int)strlen(second); ++i) {
        first[i] = second[i];
    }
}

regex_t** convert_to_regex_t(char **templates, int i_flag) {
    regex_t **patterns = NULL;
    patterns = (regex_t**)malloc(sizeof(regex_t*));
    for (int i = 0; *(templates + i) != NULL; ++i) {
        patterns = (regex_t**)realloc(patterns, sizeof(regex_t*) * (i + 1));
        patterns[i] = (regex_t*)malloc(sizeof(regex_t));
        if (i_flag) {
            regcomp(patterns[i], *(templates + i), REG_ICASE);
        } else {
            regcomp(patterns[i], *(templates + i), REG_EXTENDED);
        }
    }
    return patterns;
}

char** association_templates(FILE *file, char **templates, int *count) {
    int k = 0;
    char buffer[2];
    while (fgets(buffer, 2, file)) {
        if (k == 0) {
            ++*count;
            if (templates == NULL) {
                templates = (char**)malloc(sizeof(char*));
            } else {
                templates = (char**)realloc(templates, sizeof(char*) * *count);
            }
        }
        if (buffer[0] != '\n') {
            ++k;
            if (templates[*count - 1] == NULL) {
                templates[*count - 1] = (char*)malloc(sizeof(char));
            } else {
                templates[*count - 1] = (char*)realloc(templates[*count - 1], sizeof(char) * k);
            }
            templates[*count - 1][k - 1] = buffer[0];
        } else {
            k = 0;
        }
    }
    return templates;
}

int check_exist_file(char **paths) {
    FILE *file = NULL;
    int k = 0, error = 1;
    for (int i = 0; error && (*(paths + i) != NULL); ++i) {
        file = fopen(*(paths + i), "r");
        if (file == NULL) {
            printf("grep: %s: No such file or directory", *(paths + i));
            error = 0;
        }
    }
    return error;
}

char **run_regulars_file(char **paths_regulars, char **templates, int *count) {
    if (check_exist_file(paths_regulars)) {
        FILE *file;
        for (int i = 0; *(paths_regulars + i) != NULL; ++i) {
            file = fopen(*(paths_regulars + i), "r");
            if (file != NULL) {
                templates = association_templates(file, templates, count);
            }
            fclose(file);
        }
    } else if (templates != NULL){
        clean_mass(templates);
    }
    return templates;
}

char **find_feflags_argument(char **argv, char **array, char flag) {
    int count = 0;
    for (int i = 1; *(argv + i) != NULL; ++i) {
        if ((strlen(argv[i]) == 2) && (argv[i][0] == '-') && (argv[i][1] == flag)) {
            ++count;
            if (array == NULL) {
                array = (char**)malloc(sizeof(char*));
            }
            array = (char**)realloc(array, sizeof(char*) * count);
            array[count - 1] = (char*)malloc(sizeof(char) * (int)strlen(argv[i + 1]));
            copy(array[count - 1], argv[i + 1]);
        }
    }

    return array;
}

char** find_first_temp(char **argv, char **templates) {
    for (int i = 1; templates == NULL; ++i) {
        if (argv[i][0] != '-') {
            templates = (char**)malloc(sizeof(char*));
            templates[0] = (char*)malloc(sizeof(char) * strlen(argv[i]));
            copy(templates[0], argv[i]);
        }
    }
    return templates;
}

regex_t** find_template(int argc, char **argv, struct flags *flags) {
    regex_t **patterns = NULL;
    int count = 0, i = 1;
    char **paths_regulars = NULL, **templates = NULL;

    if (!flags->e && !flags->f) {
        templates = find_first_temp(argv, templates);
    } else if (flags->e) {
        templates = find_feflags_argument(argv, templates, 'e');
    }
    if (flags->f) {
        paths_regulars = find_feflags_argument(argv, paths_regulars, 'f');
        templates = run_regulars_file(paths_regulars, templates, &count);
        clean_mass(paths_regulars);
    }
    if (templates != NULL) {
        patterns = convert_to_regex_t(templates, flags->i);
        clean_mass(templates);
    }
    return patterns;
}

#endif