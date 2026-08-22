#ifndef KFILE_H
#define KFILE_H

#include "kbuffer.h"
#include "kmalloc.h"
#include "kstring.h"
#include "ktypes.h"

#include <stddef.h>
#include <stdio.h>

typedef enum {
  KFILE_READ,
  KFILE_WRITE,
  KFILE_APPEND,
  KFILE_READ_WRITE
} KFileMode;

typedef struct {
  usize size;
  uint64_t created_at;
  uint64_t modified_at;
  bool exists;
  bool is_directory;
  bool is_readonly;
} KFileStat;

typedef struct {
  KString name;
  KString path;
  KFileStat stat;

  FILE *handle;
} KFile;

KFileStat kfilestat_new(const char *path);

KFile *kfile_new();
KFile *kfile_from(const char *path);
KFile *kfile_open(const char *path, KFileMode mode);
void kfile_close(KFile *file);

size_t kfile_len(KFile* file);
const KString* kfile_name(KFile* f);
const KString* kfile_path(KFile* f);
const KFileStat* kfile_stat(KFile* f);


bool kfile_create(KFile *file);
bool kfile_delete(KFile *file);
bool kfile_read_raw(KFile *file, void *buffer, usize size);
KBuffer* kfile_read(KFile* f);
bool kfile_write_raw(KFile *file, const void *buffer, usize size);
bool kfile_write(KFile *file, KBuffer* b);
bool kfile_rename(KFile *file, const char *path);

#endif
