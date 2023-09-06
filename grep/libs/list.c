#include "list.h"
#include "../../common/xalloc.h"
#include <stdio.h>

typedef struct snode {
    void* el;
    struct snode* next;
} snode;

snode* head = NULL;
int size = 0;

char debug = 0;

void list_print() {
    snode* p = head;
    for (; p->next; p = p->next)
        printf("%p->", p);
    printf("%p->(null)\n", p);
}
void list_destroy() {
    if (head) {
        snode *f = head;
        while (f->next) {
            snode *b = f;
            f = f->next;
            xfree(b);
        }
        xfree(f);
        head = NULL;
    }
    if (debug)
        printf("Successfully deallocated all list memory\n");
}
int list_size() {
    return size;
}

void create_head(void* data) {
    head = xmalloc(sizeof(snode));
    head->el = data;
    head->next = NULL;
    if (debug)
        printf("Created head at %p\n", head);
    size = 1;
}
void list_push(void* data) {
    if (head) {
        snode* p = head;
        for (; p->next; p = p->next) { }
        p->next = xmalloc(sizeof(snode));
        p->next->el = data;
        p->next->next = NULL;
        if (debug)
            printf("Created node at %p\n", p->next);
        size++;
    } else {
        create_head(data);
    }
}
void* list_pop() {
    if (!size)
        return NULL;
    void* r;
    if (head->next) {
        snode* p = head;
        for (; p->next->next; p = p->next) { }
        r = p->next->el;
        xfree(p->next);
        p->next = NULL;
    } else {
        r = head->el;
        xfree(head);
        head = NULL;
    }
    size--;
    return r;
}
void* list_index(int index) {
    int i = 0;
    snode* p = head;
    if (i >= size)
        return NULL;
    for (; p && i < index; p = p->next)
        i++;
    return p->el;
}
