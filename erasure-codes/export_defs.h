#ifndef ERASURE_EXPORT_DEFS_H
#define ERASURE_EXPORT_DEFS_H

#define ERASURE_SHARED

#if defined(__arm__) || defined(__TARGET_ARCH_ARM)
#if defined(__ARM_ARCH_7__) \\
        || defined(__ARM_ARCH_7A__) \\
        || defined(__ARM_ARCH_7R__) \\
        || defined(__ARM_ARCH_7M__) \\
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 7)
#define ERASURE_ARCH_ARMV7
#elif defined(__ARM_ARCH_6__) \\
        || defined(__ARM_ARCH_6J__) \\
        || defined(__ARM_ARCH_6T2__) \\
        || defined(__ARM_ARCH_6Z__) \\
        || defined(__ARM_ARCH_6K__) \\
        || defined(__ARM_ARCH_6ZK__) \\
        || defined(__ARM_ARCH_6M__) \\
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 6)
#define ERASURE_ARCH_ARMV6
#elif defined(__ARM_ARCH_5TEJ__) \\
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 5)
#define ERASURE_ARCH_ARMV5
#else
#define ERASURE_ARCH_ARM
#endif
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
#define ERASURE_ARCH_I386
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
#define ERASURE_ARCH_X86_64
#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
#define ERASURE_ARCH_IA64
#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) \\
      || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC)  \\
      || defined(_M_MPPC) || defined(_M_PPC)
#if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
#define ERASURE_ARCH_PPC64
#else
#define ERASURE_ARCH_PPC
#endif
#else
#define ERASURE_ARCH_UNKNOWN
#endif

#if defined ERASURE_SHARED && defined _WIN32
#	ifdef ERASURE_BUILD
#		define ERASURE_EXPORT __declspec(dllexport)
#	else
#		define ERASURE_EXPORT __declspec(dllimport)
#	endif
#else
#	define ERASURE_EXPORT
#endif

#endif
