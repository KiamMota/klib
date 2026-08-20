//    MIT License
//
//    Copyright (c) 2026 Kiam Mota
//
//    Permission is hereby granted, free of charge, to any person obtaining a
//    copy of this software and associated documentation files (the "Software"),
//    to deal in the Software without restriction, including without limitation
//    the rights to use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in
//    all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//    DEALINGS IN THE SOFTWARE.

#include "kstring.h"
#include "kmalloc.h"

KString *kstring_new(void) {

  KString *_str = KMALLOC(KString);
  if (!_str)
    return NULL;
  _str->_data_ = KMALLOC(char);
  if (!_str->_data_) {
    return NULL;
    free(_str);
  }
  _str->_len_ = 0;
  _str->_data_[_str->_len_] = '\0';
  return _str;
}

KString kstring_init(void) {
  KString str = {0};

  str._len_ = 0;
  str._data_ = KCALLOC(char, 1);

  return str;
}

int kstring_free(KString **km_str) {
  if (!*km_str)
    return 0;
  free(*km_str);
  *km_str = NULL;
  return 0;
}

size_t kstring_len(const KString *str) { return str->_len_; }

KString *kstring_from(const char *str) {
  KString *_str = kstring_new();
  if (str == NULL) {
    return _str;
  }
  _str->_len_ = _km_string_get_len(str);
  _km_string_cpy(_str, str);
  return _str;
}

bool kstring_nullorempty(KString *str) {
  return (str->_len_ == 0 && str->_data_[str->_len_] == '\0') ? 1 : 0;
}

void kstring_replace(KString *str, char c, char p) {
  for (int i = 0; i < str->_len_; i++) {
    if (str->_data_[i] == c) {
      str->_data_[i] = p;
    }
  }
}

KString kstring_from_s(const char *str) {
  KString result = {._data_ = NULL, ._len_ = 0};

  if (!str)
    return result;

  size_t len = _km_string_get_len(str);

  result._data_ = KMALLOC_N(char, len + 1);
  if (!result._data_)
    return result;

  for (size_t i = 0; i < len; i++)
    result._data_[i] = str[i];

  result._data_[len] = '\0';
  result._len_ = len;

  return result;
}

bool kstring_clear(KString *str) {
  char *tmp = KREALLOC(str->_data_, char, 1);
  if (!tmp) {
    return 1;
  }
  str->_data_ = tmp;
  str->_len_ = 0;
  str->_data_[str->_len_] = '\0';
  return 0;
}

bool kstring_append(KString *str, const char *src) {
  if (!str || !src)
    return false;

  size_t src_sz = _km_string_get_len(src);
  size_t new_size = str->_len_ + src_sz + 1;

  char *tmp = KREALLOC(str->_data_, char, new_size);

  if (!tmp)
    return false;

  str->_data_ = tmp;

  for (size_t i = 0; i < src_sz; i++) {
    str->_data_[str->_len_ + i] = src[i];
  }

  str->_len_ += src_sz;
  str->_data_[str->_len_] = '\0';

  return true;
}

bool kstring_push(KString *str, char c) {
  size_t new_sz = str->_len_ + 2;
  char *tp = KREALLOC(str->_data_, char, new_sz);
  if (!tp)
    return 1;
  str->_data_ = tp;
  str->_data_[str->_len_] = c;
  str->_len_++;
  str->_data_[str->_len_] = '\0';
  return 0;
}

bool kstring_push_str(KString *str, const char *s) {
  size_t s_size = _km_string_get_len(s);
  size_t new_size = str->_len_ + s_size + 1;
  char *p = KREALLOC(str->_data_, char, new_size);
  if (!p)
    return false;
  str->_data_ = p;
  for (size_t i = 0; i < s_size; i++) {
    str->_data_[str->_len_] = s[i];
    str->_len_++;
  }
  str->_data_[new_size] = '\0';
  return true;
}

void kstring_tolower(KString *name) {
  for (size_t i = 0; i < name->_len_; i++) {
    if (_km_string_is_uppercase(name->_data_[i])) {
      name->_data_[i] = _km_string_to_lower(name->_data_[i]);
    }
  }
}

KString *kstring_lower(const KString *str) {
  if (!str)
    return NULL;

  KString *new_str = kstring_from(kstring_cstr(str));
  if (!new_str)
    return NULL;

  for (size_t i = 0; i < new_str->_len_; i++) {
    if (_km_string_is_uppercase(new_str->_data_[i])) {
      new_str->_data_[i] = _km_string_to_lower(new_str->_data_[i]);
    }
  }

  return new_str;
}

void kstring_toupper(KString *name) {
  for (size_t i = 0; i < name->_len_; i++) {
    if (_km_string_is_lowercase(name->_data_[i])) {
      name->_data_[i] = _km_string_to_upper(name->_data_[i]);
    }
  }
}

KString *k_string_uppercase(const KString *str) {
  if (!str)
    return NULL;

  KString *new_str = KMALLOC(KString);
  if (!new_str)
    return NULL;

  new_str->_len_ = str->_len_;
  new_str->_data_ = KREALLOC(NULL, char, str->_len_ + 1);

  if (!new_str->_data_) {
    free(new_str);
    return NULL;
  }

  for (size_t i = 0; i < str->_len_; i++) {
    char c = str->_data_[i];

    if (_km_string_is_lowercase(c))
      c = _km_string_to_upper(c);

    new_str->_data_[i] = c;
  }

  new_str->_data_[str->_len_] = '\0';

  return new_str;
}

bool kstring_countains(const KString *str, const char *sub) {
  if (!sub || sub == NULL)
    return false;
  for (size_t i = 0; str->_data_[i] != '\0'; i++) {
    size_t j = 0;
    while (sub[j] != '\0' && str->_data_[i + j] == sub[j]) {
      j++;
    }
    if (sub[j] == '\0') {
      return true;
    }
  }
  return false;
}

bool kstring_trim(KString *str) {
  if (!str || !str->_data_)
    return false;

  size_t j = 0;

  for (size_t i = 0; i < str->_len_; i++) {
    if (str->_data_[i] != ' ') {
      str->_data_[j++] = str->_data_[i];
    }
  }

  str->_data_[j] = '\0';
  str->_len_ = j;

  return true;
}

const char *kstring_cstr(KString *str) { return str->_data_; }
