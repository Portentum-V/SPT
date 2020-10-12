#pragma once
#ifndef CPUID_H
#define CPUID_H

/**
 * @brief   Contains a portable cpuid implementation for x86 and
 *          x86-64 CPUs.
 * @author  Kim Walisch, <kim.walisch@gmail.com>
 * @version 1.1
 * 
 * Shamelessly stolen:
 *  https://github.com/llazzaro/chem-fingerprints/blob/master/src/cpuid.h
 */

#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
#include <intrin.h> /* __cpuid() */
#endif

 /* %ecx bit flags */
#define bit_SSE3    (1 <<  0)
#define bit_SSSE3   (1 <<  9)
#define bit_SSE4_1  (1 << 19)
#define bit_SSE4_2  (1 << 20)
#define bit_POPCNT  (1 << 23)
#define bit_AVX     (1 << 28)

/* %edx bit flags */
#define bit_SSE     (1 << 25)
#define bit_SSE2    (1 << 26)

/**
 * Portable cpuid implementation for x86 and x86-64 CPUs
 * (supports PIC and non-PIC code).
 * @return  1 if the CPU supports the cpuid instruction else -1.
 */
static int cpuid(unsigned int info,
    unsigned int* eax,
    unsigned int* ebx,
    unsigned int* ecx,
    unsigned int* edx)
{
#if defined(_MSC_VER) && (defined(_WIN32) || defined(_WIN64))
    int regs[4];
    __cpuid(regs, info);
    *eax = regs[0];
    *ebx = regs[1];
    *ecx = regs[2];
    *edx = regs[3];
    return 1;
#elif defined(__i386__) || defined(__i386)
    *eax = info;
#if defined(__PIC__)
    __asm__ __volatile__(
        "mov %%ebx, %%esi;" /* save %ebx PIC register */
        "cpuid;"
        "xchg %%ebx, %%esi;"
        : "+a" (*eax),
        "=S" (*ebx),
        "=c" (*ecx),
        "=d" (*edx));
#else
    __asm__ __volatile__(
        "cpuid;"
        : "+a" (*eax),
        "=b" (*ebx),
        "=c" (*ecx),
        "=d" (*edx));
#endif
    return 1;
#elif defined(__x86_64__)
    *eax = info;
    __asm__ __volatile__(
        "cpuid;"
        : "+a" (*eax),
        "=b" (*ebx),
        "=c" (*ecx),
        "=d" (*edx));
    return 1;
#else
    /* compiler or CPU architecture do not support cpuid. */
    UNUSED(info);
    UNUSED(eax);
    UNUSED(ebx);
    UNUSED(ecx);
    UNUSED(edx);
    return -1;
#endif
}

/**
 * @return  An int value with the SSE and AVX bit flags set if the CPU
 *          supports the corresponding instruction sets.
 */
static int get_cpuid_flags(void)
{
    char vendor_str[13] = { 0 };
    char model_str[49] = { 0 };
    unsigned __int32 model[12] = { 0 };
    int i = 0;

    int flags = 0;
    unsigned int info = 0x00000001;
    unsigned int eax, ebx, ecx, edx;

    if (cpuid(info, &eax, &ebx, &ecx, &edx) != -1) {
        flags = (edx & (bit_SSE |
            bit_SSE2)) |
            (ecx & (bit_SSE3 |
                bit_SSSE3 |
                bit_SSE4_1 |
                bit_SSE4_2 |
                bit_POPCNT |
                bit_AVX));
    }

    info = 0x00000000;
    if (cpuid(info, &eax, &model[0], &model[2], &model[1]) != -1) {
        /* printf("Vendor (raw): %X %X %X\n", model[0], model[2], model[1]);
        printf("Vendor: %s\n", model); */
        strncpy_s(vendor_str, 13, model, 12);
    }

    printf("Vendor Str: %s\n", vendor_str);

    /* printf("Model (raw): "); */
    for (info = 0x80000002; info < 0x80000005; info++) {
        if (cpuid(info, &model[i], &model[i+1], &model[i+2], &model[i+3]) != -1) {
            /* printf("%X %X %X %X  ", model[i], model[i+1], model[i+2], model[i + 3]); */
            i += 4;
        }
        else {
            printf("Error retieving CPU model information.\n");
            break;
        }
    }
    /* printf("\nModel: %s\n", model); */
    strncpy_s(model_str, 49, model, 48);
    printf("Model Str: %s\n", model_str);

    return flags;
}

#endif /* CPUID_H */
