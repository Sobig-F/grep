#ifndef SORT_H
#define SORT_H

#include <regex.h>
#include <stdio.h>

void swap(regmatch_t *a, regmatch_t *b) {
  regmatch_t temp;
  temp.rm_so = a->rm_so;
  temp.rm_eo = a->rm_eo;
  a->rm_so = b->rm_so;
  a->rm_eo = b->rm_eo;
  b->rm_so = temp.rm_so;
  b->rm_eo = temp.rm_eo;
}

void quick_sort(regmatch_t **a, int n) {
  if (n > 0) {
    for (int i = 0; i < n; ++i) {
      if ((*(a + i))->rm_so < (*a)->rm_so) {
        swap(*a, *(a + i));
      }
    }
    quick_sort(a + 1, n - 1);
  }
}

#endif