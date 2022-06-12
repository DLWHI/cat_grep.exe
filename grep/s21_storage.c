#include "s21_storage.h"
#include <stdio.h>
#include "../common/xalloc.h"

long ssize = 0;
char* door = NULL;
char* wall = NULL;
char* p  = NULL;

int strcmp_local(char* ptr1, char* ptr2) {
    while (*ptr1 && (*ptr1 == *ptr2)) {
        ptr1++;
        ptr2++;
    }
    return (*ptr1 - *ptr2);
}
int exists(char* byte_seq) {
    char* pbegin = door, *pend = pbegin;
    while (pend && pend != p) {
        for (; *pend != '\n' && *pend != '\0'; pend++) { }
        *pend = '\0';
        if (!strcmp_local(byte_seq, pbegin)) {
            *pend = '\n';
            return 1;
        }
        *pend = '\n';
        pbegin = ++pend;
    }
    return  0;
}

void storage_refit() {
    door = xrealloc(door, ssize*2);
    p = door + ssize;
    ssize *= 2;
}
void storage_put(char* tail) {
    if (!*tail)
        tail = ".\\*";
    if (exists(tail))
        return;
    if (!door) {
        ssize = 32;
        door = xmalloc(ssize);
        p = door;
        *p = '\0';
    }
    while (*tail) {
        if (p - door == ssize)
            storage_refit();
        *p++ = *tail++;
    }
    if (p - door == ssize)
        storage_refit();
    *p++ = '\n';
    wall = p - 1;
}
void storage_seek(int pos, long offs) {
    if (pos == S_BEG)
        p = door + offs;
    else if (pos == S_CUR)
        p += offs;
    else if (pos == S_END)
        p = wall - offs;
}
int storage_size() {
    return ssize;
}
char* storage_get(int pos) {
    if (pos == S_BEG)
        return door;
    else if (pos == S_CUR)
        return p;
    else if (pos == S_END)
        return wall;
    return NULL;
}
void storage_destroy() {
    xfree(door);
    door = p = NULL;
}
