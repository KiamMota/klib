#ifndef KLIST_DEF
#define KLIST_DEF

#include "ktypes.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void  **items;
    usize   count;
    usize   capacity;
} KList;

KList *klist_new(void);
void   klist_free(KList **l);
KList  klist_init(void);
void   klist_uninit(KList *l);

bool   klist_push(KList *l, void *item);
void  *klist_pop(KList *l);
void  *klist_get(KList *l, usize index);
bool   klist_remove(KList *l, usize index);
void   klist_clear(KList *l);

usize  klist_len(KList *l);
bool   klist_empty(KList *l);

#endif
