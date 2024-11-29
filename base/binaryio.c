#include "binaryio.h"

#include "file.h"
#include "panic.h"
#include <stdlib.h>
#include <string.h>

BinaryReader binary_reader_from_bytes(u8 *data, usize length, bool take_copy,
                                      bool big_endian) {
  BinaryReader reader = {
      .data = NULL, .length = length, .offset = 0, .is_big_endian = big_endian};
  if (take_copy) {
    reader.data = malloc_or_panic(length);
    memcpy(reader.data, data, length);
  } else {
    reader.data = data;
  }
  return reader;
}

BinaryReader binary_reader_from_file(char const *path, bool big_endian) {
  usize length = 0;
  u8 *data = read_file_binary(path, &length);
  if (data == NULL)
    panic("File read failed");
  return binary_reader_from_bytes(data, length, false, big_endian);
}

u8 *binary_reader_copy_bytes(BinaryReader *reader, usize length) {
  if (reader->offset + length >= reader->length)
    panic("Unexpected end of file");
  u8 *buffer = malloc(length);
  memcpy(buffer, &reader->data[reader->offset], length);
  buffer[length] = 0;
  reader->offset += length;
  return buffer;
}

u8 binary_reader_read_u8(BinaryReader *reader) {
  if (reader->offset >= reader->length)
    panic("Unexpected end of file");
  return reader->data[reader->offset++];
}

u16 binary_reader_read_u16(BinaryReader *reader) {
  u16 first = binary_reader_read_u8(reader);
  u16 second = binary_reader_read_u8(reader);
  if (reader->is_big_endian) {
    return (first << 8) | second;
  } else {
    return (second << 8) | first;
  }
}

u32 binary_reader_read_u32(BinaryReader *reader) {
  u32 first = binary_reader_read_u16(reader);
  u32 second = binary_reader_read_u16(reader);
  if (reader->is_big_endian) {
    return (first << 16) | second;
  } else {
    return (second << 16) | first;
  }
}

u64 binary_reader_read_u64(BinaryReader *reader) {
  u64 first = binary_reader_read_u32(reader);
  u64 second = binary_reader_read_u32(reader);
  if (reader->is_big_endian) {
    return (first << 32) | second;
  } else {
    return (second << 32) | first;
  }
}

i8 binary_reader_read_i8(BinaryReader *reader) {
  u8 value = binary_reader_read_u8(reader);
  return REINTERPRET_CAST(i8, value);
}

i16 binary_reader_read_i16(BinaryReader *reader) {
  u16 value = binary_reader_read_u16(reader);
  return REINTERPRET_CAST(i16, value);
}

i32 binary_reader_read_i32(BinaryReader *reader) {
  u32 value = binary_reader_read_u32(reader);
  return REINTERPRET_CAST(i32, value);
}

i64 binary_reader_read_i64(BinaryReader *reader) {
  u64 value = binary_reader_read_u64(reader);
  return REINTERPRET_CAST(i64, value);
}

f32 binary_reader_read_f32(BinaryReader *reader) {
  u32 value = binary_reader_read_u32(reader);
  return REINTERPRET_CAST(f32, value);
}

f64 binary_reader_read_f64(BinaryReader *reader) {
  u64 value = binary_reader_read_u64(reader);
  return REINTERPRET_CAST(f64, value);
}

void binary_reader_free(BinaryReader *reader) { free(reader->data); }

const usize binary_writer_default_buffer_size = 16;

BinaryWriter binary_writer_new(bool big_endian) {
  return binary_writer_with_capacity(binary_writer_default_buffer_size,
                                     big_endian);
}

BinaryWriter binary_writer_with_capacity(usize capacity, bool big_endian) {
  BinaryWriter writer = {.buffer = malloc_or_panic(capacity),
                         .capacity = capacity,
                         .offset = 0,
                         .is_big_endian = big_endian};
  return writer;
}

void binary_writer_write_u8(BinaryWriter *writer, u8 value) {
  if (writer->offset >= writer->capacity) {
    if (writer->capacity == 0) {
      writer->capacity = binary_writer_default_buffer_size;
    } else {
      writer->capacity *= 2;
    }
    writer->buffer = realloc_or_panic(writer->buffer, writer->capacity);
  }
  writer->buffer[writer->offset] = value;
  writer->offset++;
}

void binary_writer_write_u16(BinaryWriter *writer, u16 value) {
  if (writer->is_big_endian) {
    binary_writer_write_u8(writer, (0xFF00 & value) >> 8);
    binary_writer_write_u8(writer, 0x00FF & value);
  } else {
    binary_writer_write_u8(writer, 0x00FF & value);
    binary_writer_write_u8(writer, (0xFF00 & value) >> 8);
  }
}

void binary_writer_write_u32(BinaryWriter *writer, u32 value) {
  if (writer->is_big_endian) {
    binary_writer_write_u16(writer, (0xFFFF0000 & value) >> 16);
    binary_writer_write_u16(writer, 0x0000FFFF & value);
  } else {
    binary_writer_write_u16(writer, 0x0000FFFF & value);
    binary_writer_write_u16(writer, (0xFFFF0000 & value) >> 16);
  }
}

void binary_writer_write_u64(BinaryWriter *writer, u64 value) {
  if (writer->is_big_endian) {
    binary_writer_write_u32(writer, (0xFFFFFFFF00000000 & value) >> 32);
    binary_writer_write_u32(writer, 0x00000000FFFFFFFF & value);
  } else {
    binary_writer_write_u32(writer, 0x00000000FFFFFFFF & value);
    binary_writer_write_u32(writer, (0xFFFFFFFF00000000 & value) >> 32);
  }
}

void binary_writer_write_i8(BinaryWriter *writer, i8 value) {
  binary_writer_write_u8(writer, REINTERPRET_CAST(u8, value));
}

void binary_writer_write_i16(BinaryWriter *writer, i16 value) {
  binary_writer_write_u16(writer, REINTERPRET_CAST(u16, value));
}

void binary_writer_write_i32(BinaryWriter *writer, i32 value) {
  binary_writer_write_u32(writer, REINTERPRET_CAST(u32, value));
}

void binary_writer_write_i64(BinaryWriter *writer, i64 value) {
  binary_writer_write_u64(writer, REINTERPRET_CAST(u64, value));
}

void binary_writer_write_f32(BinaryWriter *writer, f32 value) {
  binary_writer_write_u32(writer, REINTERPRET_CAST(u32, value));
}
void binary_writer_write_f64(BinaryWriter *writer, f64 value) {
  binary_writer_write_u64(writer, REINTERPRET_CAST(u64, value));
}

void binary_writer_to_file(BinaryWriter *writer, char const *path) {
  write_file_binary(path, writer->buffer, writer->offset);
}

void binary_writer_free(BinaryWriter *writer) { free(writer->buffer); }
