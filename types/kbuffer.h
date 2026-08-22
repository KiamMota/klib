#ifndef KBUFFER_DEC
#define KBUFFER_DEC

#include "ktypes.h"
#include <stddef.h>

typedef struct {
  void* _data_;
  usize _size_;

} KBuffer;

KBuffer* kbuffer_new();
void kbuffer_free(KBuffer** bf);
KBuffer kbuffer_init();
KBuffer* kbuffer_from(void* data, usize sz);
usize kbuffer_len(KBuffer* bf);
void* kbuffer_data(KBuffer* bf);
bool kbuffer_clear(KBuffer* bf);

#endif
