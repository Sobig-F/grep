#ifndef FLAGS_H
#define FLAGS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct flags {
    int i, v, c, l, n, h, s, o, f, e;
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
    for (int i = 1; (i < flagsSize - 1) && validate; ++i) {
        len = strlen(*(flags + i));
        for (int j = 1; (j < len) && validate; ++j) {
            if ((flags[i][0] == '-') && (flags[i][j] != 'i') && (flags[i][j] != 'v') && (flags[i][j] != 'c') && (flags[i][j] != 'l') && (flags[i][j] != 'n') && (flags[i][j] != 'h') && (flags[i][j] != 's') && (flags[i][j] != 'o')) {
                validate = 0;
            }
            if (((flags[i][j] == 'e') && (len == 2)) || ((flags[i][j] == 'f') && (len == 2)) || (flags[i][j] == '.')) {
                validate = 1;
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
        flags->f = find_flag(argv, 'f', argc);
        flags->e = find_flag(argv, 'e', argc);
    }
    return flags;
}

#endif