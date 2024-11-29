#ifndef H_BINARY_IO
#define H_BINARY_IO

#include "types.h"

typedef struct {
  u8 *data;
  usize offset;
  usize length;
  bool is_big_endian;
} BinaryReader;

BinaryReader binary_reader_from_bytes(u8 *data, usize length, bool take_copy,
                                      bool big_endian);
BinaryReader binary_reader_from_file(char const *path, bool big_endian);

char *binary_reader_copy_string(BinaryReader *reader, usize length);

u8 binary_reader_read_u8(BinaryReader *reader);
u16 binary_reader_read_u16(BinaryReader *reader);
u32 binary_reader_read_u32(BinaryReader *reader);
u64 binary_reader_read_u64(BinaryReader *reader);

i8 binary_reader_read_i8(BinaryReader *reader);
i16 binary_reader_read_i16(BinaryReader *reader);
i32 binary_reader_read_i32(BinaryReader *reader);
i64 binary_reader_read_i64(BinaryReader *reader);

f32 binary_reader_read_f32(BinaryReader *reader);
f64 binary_reader_read_f64(BinaryReader *reader);

void binary_reader_free(BinaryReader *reader);

typedef struct {
  u8 *buffer;
  usize capacity;
  usize offset;
  bool is_big_endian;
} BinaryWriter;

BinaryWriter binary_writer_new(bool big_endian);
BinaryWriter binary_writer_with_capacity(usize capacity, bool big_endian);

void binary_writer_write_u8(BinaryWriter *writer, u8 value);
void binary_writer_write_u16(BinaryWriter *writer, u16 value);
void binary_writer_write_u32(BinaryWriter *writer, u32 value);
void binary_writer_write_u64(BinaryWriter *writer, u64 value);

void binary_writer_write_i8(BinaryWriter *writer, i8 value);
void binary_writer_write_i16(BinaryWriter *writer, i16 value);
void binary_writer_write_i32(BinaryWriter *writer, i32 value);
void binary_writer_write_i64(BinaryWriter *writer, i64 value);

void binary_writer_write_f32(BinaryWriter *writer, f32 value);
void binary_writer_write_f64(BinaryWriter *writer, f64 value);

void binary_writer_to_file(BinaryWriter *writer, char const *path);

void binary_writer_free(BinaryWriter *writer);

#endif