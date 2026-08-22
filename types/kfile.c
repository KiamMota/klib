#include "kfile.h"
#include "kbuffer.h"
#include "kmalloc.h"
#include "kstring.h"
#include "ktypes.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)

#include <windows.h>

#else

#include <sys/stat.h>
#include <unistd.h>

#endif

static const char *_kfile_basename(const char *path) {
  if (!path)
    return NULL;

  const char *slash = strrchr(path, '/');

#if defined(_WIN32)

  const char *backslash = strrchr(path, '\\');

  if (backslash && (!slash || backslash > slash))
    slash = backslash;

#endif

  return slash ? slash + 1 : path;
}

KFileStat kfilestat_new(const char *path) {
  KFileStat result = {0};

  if (!path)
    return result;

#if defined(_WIN32)

  WIN32_FILE_ATTRIBUTE_DATA data;

  if (!GetFileAttributesExA(path, GetFileExInfoStandard, &data))
    return result;

  result.exists = true;

  result.size = ((uint64_t)data.nFileSizeHigh << 32) | data.nFileSizeLow;

  ULARGE_INTEGER created;
  created.LowPart = data.ftCreationTime.dwLowDateTime;
  created.HighPart = data.ftCreationTime.dwHighDateTime;

  result.created_at = (created.QuadPart / 10000000ULL) - 11644473600ULL;

  ULARGE_INTEGER modified;
  modified.LowPart = data.ftLastWriteTime.dwLowDateTime;
  modified.HighPart = data.ftLastWriteTime.dwHighDateTime;

  result.modified_at = (modified.QuadPart / 10000000ULL) - 11644473600ULL;

  result.is_directory = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

  result.is_readonly = (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;

#else

  struct stat st;

  if (stat(path, &st) != 0)
    return result;

  result.exists = true;

  result.size = (usize)st.st_size;
  result.modified_at = (uint64_t)st.st_mtime;

#if defined(__APPLE__)

  result.created_at = (uint64_t)st.st_birthtime;

#else

  /*
   * Linux não possui uma API POSIX portátil para
   * data de criação. st_ctime não significa criação;
   * significa mudança dos metadados.
   */
  result.created_at = 0;

#endif

  result.is_directory = S_ISDIR(st.st_mode);

  result.is_readonly = access(path, W_OK) != 0;

#endif

  return result;
}

KFile *kfile_new(void) {
  KFile *file = KMALLOC(KFile);
  if (!file)
    return NULL;
  file->handle = NULL;
  file->name = kstring_init();
  file->path = kstring_init();
  file->stat = (KFileStat){0};
  return file;
}

KFile *kfile_from(const char *path) {
  if (!path)
    return NULL;
  KFile *file = kfile_new();
  if (!file)
    return NULL;
  const char *name = _kfile_basename(path);
  file->name = kstring_from_s(name);
  if (!file->name._data_) {
    KFREE(file);
    return NULL;
  }
  file->path = kstring_from_s(path);
  file->stat = kfilestat_new(path);
  return file;
}

KFile *kfile_open(const char *path, KFileMode mode) {
  if (!path)
    return NULL;

  KFile *file = kfile_from(path);

  if (!file)
    return NULL;

  const char *open_mode = NULL;

  switch (mode) {
  case KFILE_READ:
    open_mode = "rb";
    break;

  case KFILE_WRITE:
    open_mode = "wb";
    break;

  case KFILE_APPEND:
    open_mode = "ab";
    break;

  case KFILE_READ_WRITE:
    open_mode = "r+b";
    break;

  default:
    KFREE(file);
    return NULL;
  }

  file->handle = fopen(path, open_mode);

  /*
   * READ_WRITE não cria o arquivo caso ele não exista.
   * Se quiser esse comportamento, use "w+b".
   */
  if (!file->handle) {
    KFREE(file);
    return NULL;
  }

  file->stat = kfilestat_new(path);

  return file;
}

void kfile_close(KFile *file) {
  if (!file)
    return;

  if (file->handle) {
    fclose(file->handle);
    file->handle = NULL;
  }

  KFREE(file);
}
size_t kfile_len(KFile *file) { return file->stat.size; }

bool kfile_create(KFile *file) {
  if (!file)
    return false;

  const char *path = kstring_cstr(&file->path);

  if (!path)
    return false;

  FILE *handle = fopen(path, "wb");

  if (!handle)
    return false;

  fclose(handle);

  file->stat = kfilestat_new(path);

  return true;
}

bool kfile_delete(KFile *file) {
  if (!file)
    return false;

  if (file->handle)
    kfile_close(file);

  const char *path = kstring_cstr(&file->path);

  if (!path)
    return false;

#if defined(_WIN32)

  bool success = DeleteFileA(path) != 0;

#else

  bool success = unlink(path) == 0;

#endif

  if (success)
    file->stat = kfilestat_new(path);

  return success;
}

bool kfile_read(KFile *file, KBuffer* buff) {
    if (!file)
        return NULL;

    KBuffer *buf = kbuffer_from(NULL, file->stat.size);
    if (!buf)
        return NULL;

    if (!kfile_read_raw(file, kbuffer_data(buf), kbuffer_len(buf))) {
        kbuffer_free(&buf);
        return NULL;
    }

    return buf;
}

bool kfile_read_raw(KFile *file, void *buffer, usize size) {
  if (!file || !buffer || size == 0)
    return false;

  if (!file->handle)
    return false;

  usize read = fread(buffer, 1, size, file->handle);

  return read == size;
}

bool kfile_write(KFile *file, const char *string) {
  if (!file || !string)
    return false;
  return kfile_write_cstr(file, string, strlen(string));
}

bool kfile_write_cstr(KFile *file, const void *buffer, usize size) {
  if (!file || !buffer || size == 0)
    return false;

  if (!file->handle)
    return false;

  usize written = fwrite(buffer, 1, size, file->handle);

  if (written != size)
    return false;

  fflush(file->handle);

  file->stat = kfilestat_new(kstring_cstr(&file->path));

  return true;
}

bool kfile_rename(KFile *file, const char *path) {
  if (!file || !path)
    return false;

  if (file->handle)
    return false;

  const char *old_path = kstring_cstr(&file->path);

  if (!old_path)
    return false;

  if (rename(old_path, path) != 0)
    return false;

  kstring_clear(&file->path);
  kstring_push_str(&file->path, path);

  const char *name = _kfile_basename(path);

  kstring_clear(&file->name);
  kstring_push_str(&file->name, name);

  file->stat = kfilestat_new(path);

  return true;
}
