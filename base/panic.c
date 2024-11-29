#include "panic.h"
#include <stdio.h>
#include <stdlib.h>

noreturn void panic(char const *message) {
  fprintf(stderr, "%s\n", message);
  exit(EXIT_FAILURE);
}

void *malloc_or_panic(usize length) {
  void *ptr = malloc(length);
  if (ptr == NULL)
    panic("Ran out of memory");
  return ptr;
}

void *realloc_or_panic(void *pointer, usize length) {
  void *ptr = realloc(pointer, length);
  if (ptr == NULL)
    panic("Ran out of memory");
  return ptr;
}