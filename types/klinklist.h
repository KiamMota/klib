#ifndef KLINKLIST_DEF
#define KLINKLIST_DEF

#include <stddef.h>
#include <stdbool.h>

typedef struct KNodeList {
  void* data;
  struct KNodeList* next;
  struct KNodeList* prev;
} KLinkedListNode;

typedef struct {
  KLinkedListNode* head;
  size_t len;
} KLinkedList;
KLinkedList *klinklist_new(void);
void klinklist_free(KLinkedList **list);

KLinkedListNode *klinklist_new_node(void *data);
void klinklist_free_node(KLinkedListNode **node);

bool klinklist_append(KLinkedList *list, KLinkedListNode *node);
bool klinklist_prepend(KLinkedList *list, KLinkedListNode *node);

bool klinklist_remove(KLinkedList *list, KLinkedListNode *node);
KLinkedListNode *klinklist_pop(KLinkedList *list);
KLinkedListNode *klinklist_pop_front(KLinkedList *list);

KLinkedListNode *klinklist_get(KLinkedList *list, size_t index);
KLinkedListNode *klinklist_first(KLinkedList *list);
KLinkedListNode *klinklist_last(KLinkedList *list);

size_t klinklist_len(KLinkedList *list);
bool klinklist_empty(KLinkedList *list);

KLinkedListNode *klinklist_find(KLinkedList *list, void *data);
bool klinklist_contains(KLinkedList *list, void *data);

void klinklist_clear(KLinkedList *list);
void klinklist_reverse(KLinkedList *list);

#endif
