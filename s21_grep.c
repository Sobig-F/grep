#include "files.h"

int main(int argc, char * argv[]) {
    char **templates = NULL, **paths_files = NULL;
    struct flags *flags = NULL;
    paths_files = find_file(argc, argv);
    templates = find_template(argv, argc);
    flags = flagging(argc, argv);
    // for (int i = 0; *(paths_files + i) != NULL; ++i) {
    //     printf("%s\n", *(paths_files + i));
    // }
    // printf("\n\n");
    // for (int i = 0; *(templates + i) != NULL; ++i) {
    //     printf("%s\n", *(templates + i));
    // }
    // printf("\n\n");
    // if (flags != NULL) {
    //     printf("i - %d\n", flags->i);
    //     printf("v - %d\n", flags->v);
    //     printf("c - %d\n", flags->c);
    //     printf("l - %d\n", flags->l);
    //     printf("n - %d\n", flags->n);
    //     printf("h - %d\n", flags->h);
    //     printf("s - %d\n", flags->s);
    //     printf("o - %d\n", flags->o);
    // }
}