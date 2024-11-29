#include "file.h"

#include "panic.h"

#include <stdio.h>
#include <stdlib.h>

char *read_file(char const *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL)
    return NULL;
  fseek(file, 0, SEEK_END);
  usize size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buffer = malloc(size + 1);
  if (fread(buffer, 1, size, file) != size)
    return NULL;
  buffer[size] = 0;
  fclose(file);
  return buffer;
}
u8 *read_file_binary(char const *path, usize *length) {
  FILE *file = fopen(path, "rb");
  if (file == NULL)
    return NULL;
  fseek(file, 0, SEEK_END);
  usize size = ftell(file);
  fseek(file, 0, SEEK_SET);
  u8 *buffer = malloc_or_panic(size);
  if (fread(buffer, 1, size, file) != size)
    return NULL;
  fclose(file);
  *length = size;
  return buffer;
}
void write_file_binary(char const *path, u8 const *data, usize length) {
  FILE *file = fopen(path, "wb");
  if (fwrite(data, 1, length, file) != length)
    panic("File write failed");
  fclose(file);
}
