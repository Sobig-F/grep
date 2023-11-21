#include "find_struct.h"
#include "parsing_files.h"

int main(int argc, char * argv[]) {
    struct findStruct *grepStruct = NULL;
    if (argv != NULL) {
        grepStruct =  Malloced_find_struct(argc, argv);
    }
    if (grepStruct != NULL) {
        parsing_all_filepath(grepStruct);
        clean_findStruct(grepStruct);
    }

    return 0;
}