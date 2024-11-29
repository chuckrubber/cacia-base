#ifndef H_FILE
#define H_FILE

#include "types.h"

char *read_file(char const *path);

u8 *read_file_binary(char const *path, usize *length);

void write_file_binary(char const *path, u8 const *data, usize length);

#endif