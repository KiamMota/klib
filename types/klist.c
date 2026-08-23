#include "klist.h"
#include "kmalloc.h"
#include <string.h>

#define KLIST_INITIAL_CAPACITY 8

KList *klist_new(void) {
    KList *l = KMALLOC(KList);
    if (!l)
        return NULL;
    l->items    = KMALLOC_N(void *, KLIST_INITIAL_CAPACITY);
    if (!l->items) {
        KFREE(l);
        return NULL;
    }
    l->count    = 0;
    l->capacity = KLIST_INITIAL_CAPACITY;
    return l;
}

void klist_free(KList **l) {
    if (!l || !*l)
        return;
    KFREE((*l)->items);
    KFREE(*l);
    *l = NULL;
}

KList klist_init(void) {
    KList l;
    l.items    = KMALLOC_N(void *, KLIST_INITIAL_CAPACITY);
    l.count    = 0;
    l.capacity = l.items ? KLIST_INITIAL_CAPACITY : 0;
    return l;
}

void klist_uninit(KList *l) {
    if (!l)
        return;
    KFREE(l->items);
    l->items    = NULL;
    l->count    = 0;
    l->capacity = 0;
}

bool klist_push(KList *l, void *item) {
    if (!l)
        return false;
    if (l->count == l->capacity) {
        usize new_cap  = l->capacity * 2;
        void **resized = KREALLOC(l->items, void *, new_cap);
        if (!resized)
            return false;
        l->items    = resized;
        l->capacity = new_cap;
    }
    l->items[l->count++] = item;
    return true;
}

void *klist_pop(KList *l) {
    if (!l || l->count == 0)
        return NULL;
    return l->items[--l->count];
}

void *klist_get(KList *l, usize index) {
    if (!l || index >= l->count)
        return NULL;
    return l->items[index];
}

bool klist_remove(KList *l, usize index) {
    if (!l || index >= l->count)
        return false;
    memmove(
        &l->items[index],
        &l->items[index + 1],
        (l->count - index - 1) * sizeof(void *)
    );
    l->count--;
    return true;
}

void klist_clear(KList *l) {
    if (!l)
        return;
    l->count = 0;
}

usize klist_len(KList *l) {
    if (!l)
        return 0;
    return l->count;
}

bool klist_empty(KList *l) {
    if (!l)
        return true;
    return l->count == 0;
}
