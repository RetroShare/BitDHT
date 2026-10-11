/*
 * tests/bencode_bounds_test.c
 *
 * BitDHT.
 *
 * Copyright 2026 by defnax.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 *
 */

#include "bitdht/bencode.h"
/* Assertions perform the checks and must also run in release builds. */
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

/* Place the exact packet end against an inaccessible page, without a NUL. */
static be_node *decode(const char *text, size_t len)
{
#ifdef _WIN32
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    size_t page = info.dwPageSize;
    char *buffer = (char *)VirtualAlloc(NULL, page * 2, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    DWORD old;
    assert(buffer && VirtualProtect(buffer + page, page, PAGE_NOACCESS, &old));
#else
    size_t page = sysconf(_SC_PAGESIZE);
    char *buffer = (char *)mmap(NULL, page * 2, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(buffer != MAP_FAILED);
    assert(mprotect(buffer + page, page, PROT_NONE) == 0);
#endif
    assert(len <= page);
    memcpy(buffer + page - len, text, len);
    be_node *node = be_decoden(buffer + page - len, len);
#ifdef _WIN32
    VirtualFree(buffer, 0, MEM_RELEASE);
#else
    munmap(buffer, page * 2);
#endif
    return node;
}

int main(void)
{
    const char *bad[] = {"", "i", "i123", "i-", "ie", "i+e", "i-e", "i e", "i \t+e",
        "i1 e", "i1:", "i1:e", "i1xe", "1e", "1x:a", "d:ai1ee",
        "i9223372036854775808e", "i-9223372036854775809e", "123", "1:",
        "999999999999999999999999:", "l", "li1e", "d", "d1:a", "d1:ai1e", "d1:ae"};
    for (size_t i = 0; i < sizeof(bad)/sizeof(*bad); ++i)
        assert(decode(bad[i], strlen(bad[i])) == NULL);
    const char *good[] = {"i0e", "i-1e", "i9223372036854775807e", "i-9223372036854775808e",
        "0:", "3:abc", "le", "de", "li1ee", "d1:ai1ee", "d1:ald1:bi2eeee",
        "i+1e", "i 1e", "i \t\n\r\v\f+42e", "i  -42e"};
    for (size_t i = 0; i < sizeof(good)/sizeof(*good); ++i) {
        size_t len = strlen(good[i]);
        errno = ERANGE; /* A previous range error must not reject a valid number. */
        be_node *n = decode(good[i], len);
        assert(n);
        if (i == 2) assert(n->val.i == LLONG_MAX);
        if (i == 3) assert(n->val.i == LLONG_MIN);
        if (i == 11 || i == 12) assert(n->val.i == 1);
        if (i == 13) assert(n->val.i == 42);
        if (i == 14) assert(n->val.i == -42);
        be_free(n);
        for (size_t j = 0; j < len; ++j) assert(decode(good[i], j) == NULL);
    }
    /* strtoll must not accept a partial field ending at an embedded NUL. */
    const char nul_integer[] = {'i', '1', '\0', 'e'};
    const char nul_length[] = {'1', '\0', ':', 'a'};
    assert(decode(nul_integer, sizeof(nul_integer)) == NULL);
    assert(decode(nul_length, sizeof(nul_length)) == NULL);

    /* String contents are binary, unlike their numeric length prefix. */
    const char binary_string[] = {'3', ':', 'a', '\0', 'b'};
    be_node *n = decode(binary_string, sizeof(binary_string));
    assert(n && n->type == BE_STR && be_str_len(n) == 3);
    assert(memcmp(n->val.s, binary_string + 2, 3) == 0);
    be_free(n);

    /* Keep strtoll's acceptance of long whitespace and leading-zero prefixes. */
    char long_integer[260];
    long_integer[0] = 'i';
    for (size_t i = 0; i < 2; ++i) {
        memset(long_integer + 1, i ? '0' : ' ', 256);
        memcpy(long_integer + 257, "42e", 3);
        n = decode(long_integer, sizeof(long_integer));
        assert(n && n->type == BE_INT && n->val.i == 42);
        be_free(n);
        assert(decode(long_integer, sizeof(long_integer) - 1) == NULL);
    }
    assert(be_decoden(NULL, 1) == NULL);
    assert(be_decoden("i1e", -1) == NULL);
    return 0;
}
