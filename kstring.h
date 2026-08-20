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

#ifndef KM_STRING_H
#define KM_STRING_H

#include "ktypes.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  char *_data_;
  size_t _len_;
} KString;

static inline int _km_string_is_lowercase(char c) {
  return c >= 'a' && c <= 'z';
}

static inline int _km_string_is_uppercase(char c) {
  return c >= 'A' && c <= 'Z';
}

static inline char _km_string_to_upper(char c) {
  if (c >= 'a' && c <= 'z')
    return c - ('a' - 'A');

  return c;
}

static inline char _km_string_to_lower(char c) {
  if (c >= 'A' && c <= 'Z')
    return c + ('a' - 'A');
  return c;
}

static inline char _km_string_is_alpha(char c) {
  if (_km_string_is_lowercase(c))
    return 1;
  if (_km_string_is_uppercase(c))
    return 1;

  return 0;
}

static inline int _km_string_is_num(char c) { return (c >= '0' && c <= '9'); }

static inline int _km_string_to_num(char c) {
  switch (c) {
  case '0':
    return 0;
  case '1':
    return 1;
  case '2':
    return 2;
  case '3':
    return 3;
  case '4':
    return 4;
  case '5':
    return 5;
  case '6':
    return 6;
  case '7':
    return 7;
  case '8':
    return 8;
  case '9':
    return 9;
  default:
    return 0;
  }
  return 0;
}

static inline size_t _km_string_get_len(const char *str) {
  if (str == NULL)
    return 0;
  size_t i = 0;
  while (str[i] != '\0') {
    i++;
  }
  return i;
}

static inline void _km_string_cpy(KString *t, const char *src) {
  size_t i = 0;

  while (src[i] != '\0') {
    t->_data_[i] = src[i];
    i++;
  }

  t->_data_[i] = '\0';
}

KString *kstring_new();
KString kstring_init();
KString kstring_from_s(const char *str);
int kstring_free(KString **km_str);
size_t kstring_len(const KString *str);
KString *kstring_from(const char *str);

bool kstring_empty(KString *str);
bool kstring_clear(KString *str);
bool kstring_append(KString *str, const char *src);
bool kstring_push(KString *str, char c);

bool kstring_push_str(KString *str, const char *s);
bool kstring_countains(const KString *str, const char *sub);

void kstring_tolower(KString *name);
KString *kstring_lower(const KString *str);
void kstring_toupper(KString *name);
KString *k_string_uppercase(const KString *str);

void kstring_replace(KString *str, char c, char p);

bool kstring_trim(KString *str);

const char *kstring_cstr(KString *str);

#endif
