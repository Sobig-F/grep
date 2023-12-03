#include "find_struct.h"
#include "parsing_files.h"

int main(int argc, char *argv[]) {
  struct findStruct *grepStruct = NULL;
  if (argc > 3) {
    grepStruct = Malloced_find_struct(argc, argv);
  } else {
    printf("No arguments");
  }
  if (grepStruct != NULL) {
    parsing_all_filepath(grepStruct);
    clean_findStruct(grepStruct);
  }

  return 0;
}