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

#ifndef KMACROS_H
#define KMACROS_H

/* Platform detection */

#include <assert.h>

#define KLIB_ASSERT(condition) assert(condition)

#if defined(_WIN32) || defined(_WIN64)
#define KLIB_PLATFORM_WINDOWS 1
#define KLIB_PLATFORM_LINUX   0
#define KLIB_PLATFORM_MACOS   0
#define KLIB_PLATFORM_ANDROID 0

#elif defined(__ANDROID__)

#define KLIB_PLATFORM_WINDOWS 0
#define KLIB_PLATFORM_LINUX   0
#define KLIB_PLATFORM_MACOS   0
#define KLIB_PLATFORM_ANDROID 1

#elif defined(__linux__)

#define KLIB_PLATFORM_WINDOWS 0
#define KLIB_PLATFORM_LINUX   1
#define KLIB_PLATFORM_MACOS   0
#define KLIB_PLATFORM_ANDROID 0

#elif defined(__APPLE__)

#define KLIB_PLATFORM_WINDOWS 0
#define KLIB_PLATFORM_LINUX   0
#define KLIB_PLATFORM_MACOS   1
#define KLIB_PLATFORM_ANDROID 0

#else
#error "Klib: unsupported platform"
#endif


/* Platform name */

#if KLIB_PLATFORM_WINDOWS
#define KLIB_PLATFORM_NAME "Windows"

#elif KLIB_PLATFORM_LINUX
#define KLIB_PLATFORM_NAME "Linux"

#elif KLIB_PLATFORM_MACOS
#define KLIB_PLATFORM_NAME "macOS"

#elif KLIB_PLATFORM_ANDROID
#define KLIB_PLATFORM_NAME "Android"
#endif

#if KLIB_PLATFORM_WINDOWS

#if defined(KLIB_BUILD_SHARED)

#if defined(KLIB_EXPORTS)
#define KLIB_API __declspec(dllexport)
#else
#define KLIB_API __declspec(dllimport)
#endif

#else
#define KLIB_API
#endif

#else

#if defined(__GNUC__) || defined(__clang__)
#define KLIB_API __attribute__((visibility("default")))
#else
#define KLIB_API
#endif

#endif

#endif 