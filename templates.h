#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "flags.h"
#include "restring.h"

void clean_mass(char **arr) {
    for (int i = 0; arr[i] != NULL; free(arr[i - 1])) {
        ++i;
    }
    free(arr);
}

void copy(char *first, char *second) {
    for (int i = 0; i < (int)strlen(second); ++i) {
        first[i] = second[i];
    }
}

regex_t** convert_to_regex_t(char **templates, int i_flag, int count) {
    regex_t **patterns = NULL;    
    patterns = (regex_t**)malloc(sizeof(regex_t*) * (count + 1));
    patterns[count] = NULL;
    for (int i = 0; *(templates + i) != NULL; ++i) {
        patterns[i] = NULL;
        if (*(patterns + i) == NULL) {
            *(patterns + i) = (regex_t*)malloc(sizeof(regex_t));
        }
        if (templates[i] != NULL) {
            if (i_flag) {
                regcomp(patterns[i], *(templates + i), REG_ICASE);
            } else {
                regcomp(patterns[i], *(templates + i), REG_EXTENDED);
            }
        }
    }
    return patterns;
}

char** association_templates(FILE *file, char **templates, int *count, int *print_str) {
    char *str = NULL;
    str = get_string(file, 0);
    // printf("%s\n", str);
    while (str != NULL) {
        if (!*count) {
            templates = (char**)malloc(sizeof(char*));
        }
        *count += 1;
        templates = (char**)realloc(templates, sizeof(char*) * (*count));
        templates[*count - 1] = (char*)malloc(sizeof(char) * (int)strlen(str));
        copy(templates[*count - 1], str);
        free(str);
        str = get_string(file, 0);
        // printf("%s\n", str);
        if ((templates[*count - 1][0] == '\n') && ((int)strlen(templates[*count - 1]) == 1)) {
            *print_str = 0;
        }
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
            fprintf(stderr, "grep: %s: No such file or directory", *(paths + i));
            error = 0;
        } else {
            fclose(file);
        }
    }
    return error;
}

char **run_regulars_file(char **paths_regulars, char **templates, int *count, int *print_str) {
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
        templates = association_templates(file, templates, count, print_str);
        fclose(file);
    }
    return templates;
}

char** append_argument(char **array, int *count, char **argv, int indexj, int indexi) {
    if (array == NULL) {
        array = (char**)malloc(sizeof(char*));
    }
    ++(*count);
    if (indexj != (int)strlen(argv[indexi]) - 1) {
        array = (char**)realloc(array, sizeof(char*) * *count);
        array[*count - 1] = (char*)malloc(sizeof(char) * (strlen(argv[indexi]) - indexj - 1));
        for (int i = 0; i < (int)strlen(argv[indexi]) - indexj; ++i) {
            array[*count - 1][i] = argv[indexi][indexj + i + 1];
        }
    } else {
        if ((argv[indexi + 1] != NULL) && ((int)strlen(argv[indexi + 1]) > 1)) {
            array = (char**)realloc(array, sizeof(char*) * *count);
            array[*count - 1] = NULL;
            array[*count - 1] = (char*)malloc(sizeof(char) * (int)strlen(argv[indexi + 1]));
            if (array[*count - 1] != NULL) {
                copy(array[*count - 1], argv[indexi + 1]);
            } else {
                *count = 0;
            }
        } else {
            *count = 0;
            fprintf(stderr, "grep: option requires an argument -- %c\n", argv[indexi][indexj]);
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
    array[*count] = NULL;
    return array;
}

char** find_first_temp(char **argv, char **templates, int *count) {
    for (int i = 1; templates == NULL; ++i) {
        if (argv[i][0] != '-') {
            templates = (char**)malloc(sizeof(char*));
            if (templates != NULL) {
                templates[0] = NULL;
                *templates = (char*)malloc(sizeof(char) * strlen(argv[i]));
                copy(templates[0], argv[i]);
                ++(*count);
            }
        }
    }
    templates = (char**)realloc(templates, sizeof(char*) * (*count + 1));
    templates[*count] = NULL;
    return templates;
}

regex_t** find_template(char **argv, struct flags *flags, int *print_str) {
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
        templates = run_regulars_file(paths_regulars, templates, &count, print_str);
        if (paths_regulars != NULL) {
            clean_mass(paths_regulars);
        }
        // for (int i = 0; *(templates + i) != NULL; ++i) {
        //     printf("%s\n", *(templates + i));
        // }
    }
    if (templates != NULL) {
        patterns = convert_to_regex_t(templates, flags->i, count);
        clean_mass(templates);
    }
    return patterns;
}

#endif