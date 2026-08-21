#include "os.h"

KOs GetOs(void)
{
    KOs os = {
        .OS = OS_UNKNOWN,
        .Arch = ARCH_UNKNOWN,
        .Compiler = COMPILER_UNKNOWN
    };

#ifdef __linux__
    os.OS = OS_LINUX;
#elif defined(_WIN32)
    os.OS = OS_WINDOWS;
#elif defined(__APPLE__)
    os.OS = OS_MACOS;
#endif

#if defined(__x86_64__) || defined(_M_X64)
    os.Arch = ARCH_X86_64;
#elif defined(__aarch64__) || defined(_M_ARM64)
    os.Arch = ARCH_ARM64;
#endif

#ifdef __clang__
    os.Compiler = COMPILER_CLANG;
#elif defined(__GNUC__)
    os.Compiler = COMPILER_GCC;
#elif defined(_MSC_VER)
    os.Compiler = COMPILER_MSVC;
#endif

    return os;
}
