#ifndef FILES_H
#define FILES_H

#include "flags.h"
#include "templates.h"

char **append_path(char **paths, int *pathsSize, char *new_path) {
  if (*pathsSize == 0) {
    paths = (char **)malloc(sizeof(char *));
  } else {
    paths = (char **)realloc(paths, sizeof(char *) * (*pathsSize + 1));
  }
  paths[*pathsSize] =
      (char *)malloc(sizeof(char) * ((int)strlen(new_path) + 1));
  copy(paths[*pathsSize], new_path);
  ++*pathsSize;

  return paths;
}

char **find_file(int argc, char *argv[], int e, int *count_files) {
  char **paths = NULL;
  int pathSize = 0, allowed = 1, its_argument = 0;
  FILE *file = NULL;
  if (!e) {
    allowed = 0;
  }
  for (int i = 1; i < argc; ++i) {
    its_argument = 0;
    file = fopen(argv[i], "r");
    if ((argv[i - 1][0] == '-') &&
        ((argv[i - 1][strlen(argv[i - 1]) - 1] == 'e') ||
         (argv[i - 1][strlen(argv[i - 1]) - 1] == 'f'))) {
      its_argument = 1;
    }
    for (int j = 1; (j + 1 < (int)strlen(argv[i - 1])) && its_argument; ++j) {
      if ((argv[i - 1][j] == 'f') || (argv[i - 1][j] == 'e')) {
        its_argument = 0;
      }
    }
    if (argv[i][0] == '-') {
      its_argument = 1;
    }
    if ((file != NULL) && !its_argument) {
      if (allowed) {
        paths = append_path(paths, &pathSize, argv[i]);
      }
    } else if ((file == NULL) && !its_argument && allowed) {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
    }
    if (!allowed && (argv[i][0] != '-')) {
      allowed = 1;
    }
    if (file != NULL) {
      fclose(file);
    }
  }
  if (paths != NULL) {
    paths = (char **)realloc(paths, sizeof(char *) * (pathSize + 1));
    paths[pathSize] = NULL;
  }
  *count_files = pathSize;
  return paths;
}

#endif