/*
 * Very simple but very effective user-space memory tester.
 * Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
 * Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Version 3 not publicly released.
 * Version 4 rewrite:
 * Copyright (C) 2004-2020 Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Licensed under the terms of the GNU General Public License version 2 (only).
 * See the file COPYING for details.
 *
 * This file contains typedefs, structure, and union definitions.
 *
 */

#ifndef MEMTESTER_TYPES_H
#define MEMTESTER_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "xprintf.h"

#define memtester_printf(format, ...) xprintf(format, ##__VA_ARGS__)
// #define memtester_printf(format, ...)
#define HAS_MEMTESTER_PRINTF 1

typedef uint32_t ul;
typedef unsigned long long ull;
typedef uint32_t volatile ulv;
typedef uint8_t volatile u8v;
typedef uint16_t volatile u16v;

struct test {
    char *name;
    int (*fp)(ulv *bufa, ulv *bufb, size_t count);
};

#ifdef __cplusplus
}
#endif

#endif