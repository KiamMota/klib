#ifndef OS_DEFINITION
#define OS_DEFINITION

#include "kmalloc.h"

typedef enum {
    OS_UNKNOWN = 0,
    OS_LINUX,
    OS_WINDOWS,
    OS_MACOS
} OS;

typedef enum {
    ARCH_UNKNOWN = 0,
    ARCH_X86,
    ARCH_X86_64,
    ARCH_ARM,
    ARCH_ARM64
} OS_ARCH;

typedef enum {
    COMPILER_UNKNOWN = 0,
    COMPILER_GCC,
    COMPILER_CLANG,
    COMPILER_MSVC
} COMPILER;

typedef struct {
    OS OS;
    OS_ARCH Arch;
    COMPILER Compiler;
} Os;
Os GetOs();

#endif