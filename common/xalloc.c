#include "xalloc.h"
#include <stdlib.h>

void* xmalloc(long size) {
    void* mem = malloc(size);
    if (mem)
        return mem;
    exit(1);
}
void* xrealloc(void* ptr, long new_size) {
    void* mem = realloc(ptr, new_size);
    if (mem)
        return mem;
    exit(1);
}
void* xcalloc(long count, long size) {
    void* mem = calloc(count, size);
    if (mem)
        return mem;
    exit(1);
}
void xfree(void* ptr) {
    free(ptr);
}
