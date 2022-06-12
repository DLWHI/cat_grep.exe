#ifndef SRC_XALLOC_H_
#define SRC_XALLOC_H_

#ifndef NULL
#define NULL ((void*)0)
#endif

void* xmalloc(long size);
void* xrealloc(void* ptr, long new_size);
void* xcalloc(long count, long size);
void xfree(void* ptr);
#endif  // SRC_XALLOC_H_
