#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// void grep(FILE *path, int i, int v, int c, int l, int n, int h, int s, int o, char *template, ) {

// }

char** association_templates(FILE *file, char **templates) {
    int count_templates = 0, k = 0;
    char buffer[2];
    while (*templates != NULL) {
        ++count_templates;
        ++templates;
    }
    templates -= count_templates;
    ++count_templates;
    templates = (char**)realloc(templates, sizeof(char*)*count_templates);
    while (fgets(buffer, 2, file)) {
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
            ++count_templates;
            templates = (char**)realloc(templates, sizeof(char*)*count_templates);
        }
    }
    return templates;
}

int find_flag(char **flags, char flag, int argc) {
    int found = 0;
    for (int i = 1; (i < argc - 1) && (found == 0); ++i) {
        if (flags[i][0] != '-') {
            continue;
        }
        for (int j = 0; j < (int)strlen(flags[i]); ++j) {
            if (flags[i][j] == flag) {
                found = 1;
            }
        }
    }
    return found;
}

int check_flags_validate(char **flags, int flagsSize) {
    int validate = 1, len, e = -1, f = -1;
    for (int i = 1; (i < flagsSize - 1) && validate; ++i) {
        len = strlen(*(flags + i));
        if ((flags[i][0] != '-') && (e != i - 1) && (f != i - 1)) {
            validate = 0;
        } else if (flags[i][0] != '-') {
            continue;
        }
        for (int j = 1; (j < len) && validate; ++j) {
            if (((flags[i][j] == 'e') && (len != 2)) || ((flags[i][j] == 'f') && (len != 2))) {
                validate = 0;
            } else if ((flags[i][j] == 'e') && (len == 2)) {
                e = i;
            } else if ((flags[i][j] == 'f') && (len == 2)) {
                f = i;
            } else if ((flags[i][j] != 'i') && (flags[i][j] != 'v') && (flags[i][j] != 'c') && (flags[i][j] != 'l') && (flags[i][j] != 'n') && (flags[i][j] != 'h') && (flags[i][j] != 's') && (flags[i][j] != 'o')) {
                validate = 0;
            }
        }
    }
    return validate;
}

char** find_template(char **argv, int argc) {
    int count = 0, i = 1;
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
    return templates;
}

char* find_path_regulars(char **argv, int argc) {
    int successfully = 1;
    char *path;
    for (int i = 1; (i < argc - 1) && successfully; ++i) {
        if ((strlen(argv[i]) == 2) && (argv[i][1] == 'f') && (argv[i][0] == '-')) {
            path = argv[i + 1];
            successfully = 0;
        }
    }
    return path;
}

int main(int argc, char * argv[]) {
    int condition = 1, i, v, c, l, n, h, s, o;
    char **templates = NULL, *path_regulars = NULL, *path = argv[argc - 1];
    FILE *file = fopen(path, "r");
    templates = find_template(argv, argc);
    path_regulars = find_path_regulars(argv, argc);
    FILE *txt_regulats = fopen(path_regulars, "r");
    if (argc < 3) {
        printf("<3");
        condition = 0;
    }
    if (file == NULL) {
        printf("no such file");
        condition = 0;
    }
    if ((templates != NULL) || (txt_regulats != NULL)) {
        templates = association_templates(txt_regulats, templates);
    }
    if (condition && check_flags_validate(argv, argc)) {
        i = find_flag(argv, 'i', argc);
        v = find_flag(argv, 'v', argc);
        c = find_flag(argv, 'c', argc);
        l = find_flag(argv, 'l', argc);
        n = find_flag(argv, 'n', argc);
        h = find_flag(argv, 'h', argc);
        s = find_flag(argv, 's', argc);
        o = find_flag(argv, 'o', argc);
        // printf("%d %d %d %d %d %d %d %d\n", i, v, c, l, n, h, s, o);
        // if (templates != NULL) {
        //     int i = 0;
        //     while (*(templates + i) != NULL) {
        //         printf("%s\n", *(templates + i));
        //         ++i;
        //     }
        // }
    }
}