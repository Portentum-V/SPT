/* Node_OS.h: OS Checks */

#pragma once

#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
    /* Win */
    #define OS_WIN 1
    #include "..\\Platform_Win\\Node_Win.h"
#endif

#if defined(__linux__) || defined(__unix__)
    /* Nix (including BSD) */
    #define OS_NIX 1
#endif

#if defined(__sun)
    /* Solaris */
    #define OS_NIX 1
#endif

#if defined(__APPLE__) && defined(__MACH__)
    /* OSX, iOS, Darwin */
    #define OS_NIX 1
#endif

#if defined(_POSIX_VERSION)
    /* POSIX compliant */
#endif

#ifdef OS_NIX
    /* For all Nix related OSs */
    #include "..\\Platform_Nix\\Node_Nix.h"
#endif
