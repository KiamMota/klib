# klib

A minimal, composable C library. No OOP, no async, no opinions — just types, structures, and primitives that stay out of your way.

MIT License. Written by Kiam Mota.

---

## Philosophy

klib does one thing per module and does it well. If you need async, use libuv. If you need a full HTTP stack, use libcurl. klib gives you the ground to stand on.

Each module is a single `.h` and `.c` pair. Use only what you need.

---

## Installation

```sh
curl -fsSL https://raw.githubusercontent.com/kiammota/klib/main/install.sh | sh
```

This runs `cmake`, `make`, and `sudo make install`.

---

## Modules

### `ktypes.h`

Portable type aliases used across klib.

```c
isize   // ptrdiff_t — signed size
usize   // size_t    — unsigned size
uasize  // unsigned int
uchar   // unsigned char
```

---

### `KString`

Owned, heap-allocated string.

```c
KString *kstring_new();
KString  kstring_init();
KString  kstring_from_s(const char *str);
KString *kstring_from(const char *str);
int      kstring_free(KString **str);

size_t      kstring_len(const KString *str);
const char *kstring_cstr(KString *str);
bool        kstring_empty(KString *str);

bool kstring_append(KString *str, const char *src);
bool kstring_push(KString *str, char c);
bool kstring_push_str(KString *str, const char *s);
bool kstring_clear(KString *str);
bool kstring_trim(KString *str);
bool kstring_countains(const KString *str, const char *sub);

void     kstring_tolower(KString *str);
void     kstring_toupper(KString *str);
KString *kstring_lower(const KString *str);
KString *k_string_uppercase(const KString *str);

void kstring_replace(KString *str, char c, char replacement);
```

---

### `KLinkedList`

Doubly linked list over `void *` data. The list owns the nodes, not the data.

```c
KLinkedList *klinklist_new(void);
void         klinklist_free(KLinkedList **list);

KNode *klinklist_new_node(void *data);
void   klinklist_free_node(KNode **node);

bool   klinklist_append(KLinkedList *list, KNode *node);
bool   klinklist_prepend(KLinkedList *list, KNode *node);
bool   klinklist_remove(KLinkedList *list, KNode *node);
KNode *klinklist_pop(KLinkedList *list);
KNode *klinklist_pop_front(KLinkedList *list);

KNode  *klinklist_get(KLinkedList *list, size_t index);
KNode  *klinklist_first(KLinkedList *list);
KNode  *klinklist_last(KLinkedList *list);
size_t  klinklist_len(KLinkedList *list);
bool    klinklist_empty(KLinkedList *list);

KNode *klinklist_find(KLinkedList *list, void *data);
bool   klinklist_contains(KLinkedList *list, void *data);

void klinklist_clear(KLinkedList *list);
void klinklist_reverse(KLinkedList *list);
```

---

### `KFile`

File handle with metadata. Wraps `FILE *` with path, name, and stat.

```c
KFileStat kfilestat_new(const char *path);

KFile *kfile_new(void);
KFile *kfile_from(const char *path);
KFile *kfile_open(const char *path, KFileMode mode);
void   kfile_close(KFile *file);

bool kfile_create(KFile *file);
bool kfile_delete(KFile *file);
bool kfile_rename(KFile *file, const char *path);

bool kfile_read_raw(KFile *file, void *buffer, usize size);
bool kfile_read(KFile *file, KString *str, usize size);
bool kfile_write(KFile *file, const char *string);
bool kfile_write_cstr(KFile *file, const void *buffer, usize size);
```

`KFileMode`:
```c
KFILE_READ
KFILE_WRITE
KFILE_APPEND
KFILE_READ_WRITE
```

`KFileStat` fields:
```c
usize    size;
uint64_t created_at;
uint64_t modified_at;
bool     exists;
bool     is_directory;
bool     is_readonly;
```

---

### `KSocket`

TCP socket primitive. No async, no event loop — just the raw connection.

```c
KSocket *ksocket_new(const char *ip_address, int port);
bool     ksocket_free(KSocket **socket);

bool ksocket_connect(KSocket *socket);
bool ksocket_bind(KSocket *socket);
bool ksocket_listen(KSocket *socket, int backlog);
bool ksocket_close(KSocket *socket);
bool ksocket_is_connected(KSocket *socket);

bool     ksocket_send(KSocket *socket, const char *data, size_t length);
bool     ksocket_receive(KSocket *socket, char *buffer, size_t length);
KSocket *ksocket_accept(KSocket *socket, int seconds, int microseconds);
```

Works on Linux, macOS, and Windows. Platform differences are handled internally.

---

### `kmalloc.h`

Allocation primitives with overflow protection.

```c
KMALLOC(type)              // malloc(sizeof(type))
KCALLOC(type, count)       // calloc(count, sizeof(type))
KREALLOC(ptr, type, count) // realloc with overflow check
KMALLOC_N(type, count)     // malloc(count * sizeof(type))
KFREE(ptr)                 // free(ptr)
```

---

### `kmacros.h`

Platform detection and API visibility.

```c
KLIB_PLATFORM_WINDOWS
KLIB_PLATFORM_LINUX
KLIB_PLATFORM_MACOS
KLIB_PLATFORM_ANDROID
KLIB_PLATFORM_NAME  // string: "Linux", "Windows", etc.

KLIB_ASSERT(condition)
KLIB_API
```

---

## License

MIT — Copyright (c) 2026 Kiam Mota
