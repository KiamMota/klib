#ifndef KLINKLIST_DEF
#define KLINKLIST_DEF

#include <stddef.h>
#include <stdbool.h>
typedef struct KNodeList {
  void* data;
  struct KNodeList* next;
  struct KNodeList* prev;
} KNode;

typedef struct {
  KNode* head;
  size_t len;
} KLinkedList;
KLinkedList *klinklist_new(void);
void klinklist_free(KLinkedList **list);

KNode *klinklist_new_node(void *data);
void klinklist_free_node(KNode **node);

bool klinklist_append(KLinkedList *list, KNode *node);
bool klinklist_prepend(KLinkedList *list, KNode *node);

bool klinklist_remove(KLinkedList *list, KNode *node);
KNode *klinklist_pop(KLinkedList *list);
KNode *klinklist_pop_front(KLinkedList *list);

KNode *klinklist_get(KLinkedList *list, size_t index);
KNode *klinklist_first(KLinkedList *list);
KNode *klinklist_last(KLinkedList *list);

size_t klinklist_len(KLinkedList *list);
bool klinklist_empty(KLinkedList *list);

KNode *klinklist_find(KLinkedList *list, void *data);
bool klinklist_contains(KLinkedList *list, void *data);

void klinklist_clear(KLinkedList *list);
void klinklist_reverse(KLinkedList *list);

#endif
