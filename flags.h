#ifndef FLAGS_H
#define FLAGS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct flags {
    int i, v, c, l, n, h, s, o, f, e;
};

struct flags* find_flags(struct flags *flags, char *argv[], int *valid) {
    int feflag;
    for (int i = 1; *(argv + i) != NULL; ++i) {
        feflag = 0;
        for (int j = 1; !feflag && (j < (int)strlen(argv[i])) && (argv[i][0] == '-'); ++j) {
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
                flags->f = 1;
            }else if (argv[i][j] == 'e') {
                feflag = 1;
                flags->e = 1;
            } else if (!feflag) {
                printf("grep: invalid option -- %c\n", argv[i][j]);
                *valid = 0;
            }
        }
    }
    // if (flags->v > 1) {
    //     flags->v = flags->v % 2;
    // }
    if (flags != NULL) {
        if (flags->l) {
            // flags->h = 0;
            flags->o = 0;
            flags->n = 0;
            // flags->v = 0;
            
        }
        if (flags->c) {
            flags->n = 0;
            flags->o = 0;
        }
        if (flags->v) {
            flags->o = 0;
        }
    }
    return flags;
}

struct flags* flagging(char *argv[]) {
    struct flags *flags = NULL;
    int valid = 1;
    flags = (struct flags*)malloc(sizeof(struct flags));
    flags = find_flags(flags, argv, &valid);
    if (!valid) {
        free(flags);
        flags = NULL;
    }
    return flags;
}

#endif