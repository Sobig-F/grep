#ifndef FLAGS_H
#define FLAGS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct flags {
    int i, v, c, l, n, h, s, o, f, e;
};

struct flags* find_flags(char *argv[], int *valid) {
    int feflag, last_fefl;
    last_fefl = 0;
    struct flags *flags = NULL;
    if (flags == NULL) {
        flags = (struct flags*)malloc(sizeof(struct flags));
    }
    flags->i = 0;
    flags->v = 0;
    flags->c = 0;
    flags->l = 0;
    flags->n = 0;
    flags->h = 0;
    flags->s = 0;
    flags->o = 0;
    flags->f = 0;
    flags->e = 0;
    for (int i = 1; *(argv + i) != NULL; ++i) {
        feflag = 0;
        for (int j = 1; !last_fefl && !feflag && (j < (int)strlen(argv[i])) && (argv[i][0] == '-'); ++j) {
            if (argv[i][j] == 'i') {
                flags->i = 1;
            } else if (argv[i][j] == 'v') {
                flags->v = 1;
            } else if (argv[i][j] == 'c') {
                flags->c = 1;
            } else if (argv[i][j] == 'l') {
                flags->l = 1;
            } else if (argv[i][j] == 'n') {
                flags->n = 1;
            } else if (argv[i][j] == 'h') {
                flags->h = 1;
            } else if (argv[i][j] == 's') {
                flags->s = 1;
            } else if (argv[i][j] == 'o') {
                flags->o = 1;
            }else if (argv[i][j] == 'f') {
                feflag = 1;
                last_fefl = 1;
                flags->f = 1;
            }else if (argv[i][j] == 'e') {
                feflag = 1;
                last_fefl = 1;
                flags->e = 1;
            } else if (!feflag) {
                fprintf(stderr, "grep: invalid option -- %c\n", argv[i][j]);
                *valid = 0;
            }
            if (last_fefl && (j + 1 != (int)strlen(argv[i]))) {
                last_fefl = 0;
            }
        }
        if (!feflag && last_fefl) {
            last_fefl = 0;
        }
    }
    if (flags != NULL) {
        if (flags->l) {
            flags->o = 0;
            flags->n = 0;
        }
        if (flags->c) {
            flags->n = 0;
            flags->o = 0;
        }
        if (flags->v) {
            flags->o = 0;
        }
        if (flags->f) {
            flags->h = 1;
        }
    }
    return flags;
}

struct flags* flagging(char *argv[]) {
    struct flags *flags = NULL;
    int valid = 1;
    flags = find_flags(argv, &valid);
    if (!valid && (flags != NULL)) {
        free(flags);
        flags = NULL;
    }
    return flags;
}

#endif