#include "find_struct.h"
#include "parsing_files.h"

int main(int argc, char * argv[]) {
    struct findStruct *grepStruct = Create_find_struct(argc, argv);
    if (grepStruct != NULL) {
        find_patterns_in_file(grepStruct);
        clean_findStruct(grepStruct);
    }

    return 0;
}