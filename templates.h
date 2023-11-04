#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <stdio.h>
#include <string.h>
#include <regex.h>

void clean_mass(char **arr) {
    for (int i = 0; arr[i] != NULL; ++i) {
        free(arr[i]);
    }
    free(arr);
}

char** association_templates(FILE *file, char **templates, int *count) {
    int count_templates = 0, k = 0;
    char buffer[2];
    if (templates == NULL) {
        templates = (char**)malloc(sizeof(char*));
    } else {
        for (int i = 0; *(templates + i) != NULL; ++i) {
            ++count_templates;
        }
    }
    while (fgets(buffer, 2, file)) {
        if (k == 0) {
            ++count_templates;
            templates = (char**)realloc(templates, sizeof(char*) * count_templates);
        }
        if (buffer[0] != '\n') {
            ++k;
            if (templates[count_templates - 1] == NULL) {
                templates[count_templates - 1] = (char*)malloc(sizeof(char));
            } else {
                templates[count_templates - 1] = (char*)realloc(templates[count_templates - 1], sizeof(char) * k);
            }
            templates[count_templates - 1][k - 1] = buffer[0];
        } else {
            k = 0;
        }
    }
    *count = count_templates;
    return templates;
}

int check_exist_file(char **paths) {
    FILE *file = NULL;
    int k = 0, error = 1;
    while ((*(paths + k) != NULL) && error) {
        file = fopen(*(paths + k), "r");
        if (file == NULL) {
            printf("grep: %s: No such file or directory", *(paths + k));
            --error;
        }
        fclose(file);
        ++k;
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
    } else {
        clean_mass(templates);
        templates = NULL;
    }
    return templates;
}

void copy(char *first, char *second) {
    for (int i = 0; i < (int)strlen(second); ++i) {
        first[i] = second[i];
    }
}

char** find_path_regulars(char **argv, int argc) {
    char **paths = NULL;
    int pathSize = 0;
    for (int i = 1; i < argc; ++i) {
        if ((strlen(argv[i]) == 2) && (argv[i][1] == 'f') && (argv[i][0] == '-')) {
            if (paths == NULL) {
                ++pathSize;
                paths = (char**)malloc(sizeof(char*));
            } else {
                ++pathSize;
                paths = (char**)realloc(paths, sizeof(char*) * pathSize);
            }
            paths[pathSize - 1] = (char*)malloc(sizeof(char) * (int)strlen(argv[i + 1]));
            copy(paths[pathSize - 1], argv[i + 1]);
        }
    }
    ++pathSize;
    paths = (char**)realloc(paths, sizeof(char*) * pathSize);
    return paths;
}

regex_t** find_template(int argc, char **argv, struct flags *flags) {
    regex_t **patterns = NULL;
    int count = 0, i = 1;
    char **paths_regulars = NULL, **templates = NULL;
    
    
    while (i < argc - 1) {
        if ((strlen(argv[i]) == 2) && (argv[i][1] == 'e') && (argv[i][0] == '-')) {
            ++count;
            if (templates == NULL) {
                templates = (char**)malloc(sizeof(char*));
            } else {
                templates = (char**)realloc(templates, sizeof(char*) * count);
            }
            templates[count - 1] = (char*)malloc(sizeof(char) * (int)strlen(argv[i + 1]));
            copy(templates[count - 1], argv[i + 1]);
            ++i;
        }
        ++i;
    }


    paths_regulars = find_path_regulars(argv, argc);
    if (paths_regulars != NULL) {
        templates = run_regulars_file(paths_regulars, templates, &count);
    } else {
        templates = (char**)realloc(templates, sizeof(char*) * (count + 1));
    }
    for (int i = 0; *(paths_regulars + i); ++i) {
        free(*(paths_regulars + i));
    }
    free(paths_regulars);
    if (templates != NULL) {
        patterns = (regex_t**)malloc(sizeof(regex_t*));
        for (int i = 0; *(templates + i) != NULL; ++i) {
            patterns = (regex_t**)realloc(patterns, sizeof(regex_t*) * (i + 1));
            patterns[i] = (regex_t*)malloc(sizeof(regex_t));
            if (flags->i) {
                regcomp(patterns[i], *(templates + i), REG_ICASE);
            } else {
                regcomp(patterns[i], *(templates + i), REG_EXTENDED);
            }
        }
        free(templates);
    }
    return patterns;
}

#endif