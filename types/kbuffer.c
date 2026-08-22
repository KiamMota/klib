#include "kbuffer.h"
#include "kmalloc.h"
#include "ktypes.h"

KBuffer *kbuffer_new() {
    KBuffer *buf = KMALLOC(KBuffer);
    if (!buf)
        return NULL;
    buf->_data_ = KMALLOC(char);
    if (!buf->_data_) {
        KFREE(buf);
        return NULL;
    }
    return buf;
}
KBuffer kbuffer_init() {
  KBuffer buff_stack = {._data_ = NULL, ._size_ = 0};
  return buff_stack; 
}

KBuffer* kbuffer_from(void* data, usize sz) {
  KBuffer* buff = kbuffer_new();
  buff->_data_ = data;
  buff->_size_ = sz;
  return buff;
}

void kbuffer_free(KBuffer **buf) {
    if (!buf || !*buf)
        return;
    KFREE((*buf)->_data_);
    KFREE(*buf);
    *buf = NULL;
}

usize kbuffer_len(KBuffer *bf) {
  return bf->_size_;
}
void* kbuffer_data(KBuffer* bf) {
  return bf->_data_;
}

bool kbuffer_clear(KBuffer *bf) {
    if (!bf)
        return false;
    KFREE(bf->_data_);
    bf->_data_ = NULL;
    bf->_size_ = 0;
    return true;
}
