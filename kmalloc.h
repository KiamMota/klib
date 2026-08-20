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

#ifndef KMALLOC_DEF
#define KMALLOC_DEF

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static inline void *kmalloc(size_t size)
{
    return malloc(size);
}

static inline void *kcalloc(size_t count, size_t size)
{
    if (size != 0 && count > SIZE_MAX / size)
        return NULL;

    return calloc(count, size);
}

static inline void *krealloc(
    void *ptr,
    size_t count,
    size_t size
)
{
    if (size != 0 && count > SIZE_MAX / size)
        return NULL;

    return realloc(ptr, count * size);
}

static inline void *kmalloc_n(size_t count, size_t size)
{
    if (size != 0 && count > SIZE_MAX / size)
        return NULL;

    return malloc(count * size);
}

static inline void kfree(void *ptr)
{
    free(ptr);
}

#define KMALLOC(type) \
    kmalloc(sizeof(type))

#define KCALLOC(type, count) \
    kcalloc((count), sizeof(type))

#define KREALLOC(ptr, type, count) \
    krealloc((ptr), (count), sizeof(type))

#define KMALLOC_N(type, count) \
    kmalloc_n((count), sizeof(type))

#define KFREE(ptr) \
    kfree(ptr)

#endif