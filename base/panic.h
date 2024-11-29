#include "types.h"
#include <stdnoreturn.h>

noreturn void panic(char const *message);

void *malloc_or_panic(usize length);
void *realloc_or_panic(void *pointer, usize length);