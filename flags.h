#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FLAGS_H
#define FLAGS_H

struct flags {
    int i, v, c, l, n, h, s, o;
};

int find_flag(char **flags, char flag, int argc) {
    int found = 0;
    for (int i = 1; (i < argc) && (!found); ++i) {
        if (flags[i][0] != '-') {
            continue;
        }
        for (int j = 1; j < (int)strlen(flags[i]); ++j) {
            if (flags[i][j] == flag) {
                found = 1;
            }
        }
    }
    return found;
}

int check_flags_validate(char **flags, int flagsSize) {
    int validate = 1, len;
    //  e = -1, f = -1
    for (int i = 1; (i < flagsSize - 1) && validate; ++i) {
        len = strlen(*(flags + i));
        // if ((flags[i][0] != '-') && (e != i - 1) && (f != i - 1)) {
        //     validate = 0;
        // } else if (flags[i][0] != '-') {
        //     continue;
        // }
        for (int j = 1; (j < len) && validate; ++j) {
            if (((flags[i][j] == 'e') && (len != 2)) || ((flags[i][j] == 'f') && (len != 2))) {
                validate = 0;
            // } else if ((flags[i][j] == 'e') && (len == 2)) {
            //     e = i;
            // } else if ((flags[i][j] == 'f') && (len == 2)) {
            //     f = i;
            } else if ((flags[i][j] != 'i') && (flags[i][j] != 'v') && (flags[i][j] != 'c') && (flags[i][j] != 'l') && (flags[i][j] != 'n') && (flags[i][j] != 'h') && (flags[i][j] != 's') && (flags[i][j] != 'o')) {
                validate = 0;
            }
        }
    }
    return validate;
}

struct flags* flagging(int argc, char *argv[]) {
    struct flags *flags = NULL;
    if (check_flags_validate(argv, argc)) {
        flags = (struct flags*)malloc(sizeof(struct flags));
        flags->i = find_flag(argv, 'i', argc);
        flags->v = find_flag(argv, 'v', argc);
        flags->c = find_flag(argv, 'c', argc);
        flags->l = find_flag(argv, 'l', argc);
        flags->n = find_flag(argv, 'n', argc);
        flags->h = find_flag(argv, 'h', argc);
        flags->s = find_flag(argv, 's', argc);
        flags->o = find_flag(argv, 'o', argc);
    }
    return flags;
}

void copy(char *first, char *second) {
    for (int i = 0; i < (int)strlen(second); ++i) {
        first[i] = second[i];
    }
}

char** association_templates(FILE *file, char **templates) {
    int count_templates = 0, k = 0;
    char buffer[2];
    while (templates[count_templates] != NULL) {
        ++count_templates;
    }
    while (fgets(buffer, 2, file)) {
        if (k == 0) {
            ++count_templates;
            templates = (char**)realloc(templates, sizeof(char*)*count_templates);
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
    return templates;
}

int check_exist_file(char **paths) {
    FILE *file;
    int k = 0, error = 1;
    while (*(paths + k) != NULL) {
        file = fopen(*(paths + k), "r");
        if (file == NULL) {
            printf("%s: No such file or directory", *(paths + k));
            --error;
        }
        ++k;
    }
    return error;
}

char **run_regulars_file(char **paths_regulars, char **templates) {
    if (check_exist_file(paths_regulars)) {
        FILE *file;
        for (int i = 0; *(paths_regulars + i) != NULL; ++i) {
            file = fopen(*(paths_regulars + i), "r");
            templates = association_templates(file, templates);
        }
    }
    return templates;
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

char** find_template(char **argv, int argc) {
    int count = 0, i = 1;
    char **paths_regulars = NULL;
    char **templates = (char**)malloc(sizeof(char*));
    while (i < argc - 1) {
        if ((strlen(argv[i]) == 2) && (argv[i][1] == 'e') && (argv[i][0] == '-')) {
            ++count;
            templates = (char**)realloc(templates, sizeof(char*) * count);
            templates[count - 1] = argv[i + 1];
            ++i;
        }
        ++i;
    }
    paths_regulars = find_path_regulars(argv, argc);
    templates = run_regulars_file(paths_regulars, templates);
    return templates;
}

#endif