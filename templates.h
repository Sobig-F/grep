#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "flags.h"
#include "restring.h"

void clean_mass(char **arr) {
    for (int i = 0; arr[i] != NULL; ++i) {
        free(arr[i - 1]);
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
        patterns = (regex_t**)realloc(patterns, sizeof(regex_t*) * (i + 2));
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
    char *str = get_string(file, 0);
    while (str != NULL) {
        if (!*count) {
            templates = (char**)malloc(sizeof(char*));
        }
        *count += 1;
        templates = (char**)realloc(templates, sizeof(char*) * (*count));
        templates[*count - 1] = (char*)malloc(sizeof(char) * (int)strlen(str));
        copy(templates[*count - 1], str);
        str = get_string(file, 0);
    }
    templates = (char**)realloc(templates, sizeof(char*) * (*count + 1));
    return templates;
}

int check_exist_file(char **paths) {
    FILE *file = NULL;
    int error = 1;
    for (int i = 0; error && (*(paths + i) != NULL); ++i) {
        file = fopen(*(paths + i), "r");
        if (file == NULL) {
            printf("grep: %s: No such file or directory", *(paths + i));
            error = 0;
        } else {
            fclose(file);
        }
    }
    return error;
}

char **run_regulars_file(char **paths_regulars, char **templates, int *count) {
    int valid = 1;
    FILE *file;
    if (!check_exist_file(paths_regulars)) {
        valid = 0;
        if (templates != NULL) {
            clean_mass(templates);
        }
    }
    for (int i = 0; valid && (*(paths_regulars + i) != NULL); ++i) {
        file = fopen(*(paths_regulars + i), "r");
        templates = association_templates(file, templates, count);
        fclose(file);
    }
    return templates;
}

char** append_argument(char **array, int *count, char **argv, int indexj, int indexi) {
    if (*count == 0) {
        array = (char**)malloc(sizeof(char*));
    }
    if (indexj != (int)strlen(argv[indexi]) - 1) {
        ++(*count);
        array = (char**)realloc(array, sizeof(char*) * *count);
        array[*count - 1] = (char*)malloc(sizeof(char) * (strlen(argv[indexi]) - indexj - 1));
        for (int i = 0; i < (int)strlen(argv[indexi]) - indexj; ++i) {
            array[*count - 1][i] = argv[indexi][indexj + i + 1];
        }
    } else {
        if (argv[indexi + 1] != NULL) {
            ++(*count);
            array = (char**)realloc(array, sizeof(char*) * *count);
            array[*count - 1] = (char*)malloc(sizeof(char) * strlen(argv[indexi]));
            copy(array[*count - 1], argv[indexi + 1]);
        } else {
            *count = 0;
            printf("grep: option requires an argument -- %c\n", argv[indexi][indexj]);
        }
    }
    return array;
}

char **find_feflags_argument(char **argv, char **array, char flag, int *count) {
    int found;
    for (int i = 1; *(argv + i) != NULL; ++i) {
        found = 0;
        for (int j = 0; (j < (int)strlen(argv[i])) && (argv[i][0] == '-') && !found; ++j) {
            if (((flag == 'e') && (argv[i][j] == 'f')) || ((flag == 'f') && (argv[i][j] == 'e'))) {
                found = 1;
            }
            if ((argv[i][j] == flag) && !found) {
                found = 1;
                array = append_argument(array, count, argv, j, i);
            }
        }
    }
    array = (char**)realloc(array, sizeof(char*) * (*count + 1));
    return array;
}

char** find_first_temp(char **argv, char **templates, int *count) {
    for (int i = 1; templates == NULL; ++i) {
        if (argv[i][0] != '-') {
            templates = (char**)malloc(sizeof(char*));
            templates[0] = (char*)malloc(sizeof(char) * strlen(argv[i]));
            copy(templates[0], argv[i]);
            ++(*count);
        }
    }
    templates = (char**)realloc(templates, sizeof(char*) * (*count + 1));
    return templates;
}

regex_t** find_template(char **argv, struct flags *flags) {
    regex_t **patterns = NULL;
    int count = 0, count_path = 0;
    char **paths_regulars = NULL, **templates = NULL;
    
    if (!flags->e && !flags->f) {
        templates = find_first_temp(argv, templates, &count);
    } else if (flags->e) {
        templates = find_feflags_argument(argv, templates, 'e', &count);
    }
    if (flags->e && !count) {
        free(templates);
        templates = NULL;
        flags->f = 0;
    }
    if (flags->f) {
        paths_regulars = find_feflags_argument(argv, paths_regulars, 'f', &count_path);
        templates = run_regulars_file(paths_regulars, templates, &count);
        clean_mass(paths_regulars);
    }
    if (templates != NULL) {
        // printf("%s\n", templates[0]);
        // for (int i = 0; *(templates + i) != NULL; ++i) {
        //     printf("%s\n", *(templates + i));
        // }
        patterns = convert_to_regex_t(templates, flags->i);
        // if (patterns[1] == NULL) {
        //     printf("----------");
        // }
        clean_mass(templates);
    }
    return patterns;
}

#endif