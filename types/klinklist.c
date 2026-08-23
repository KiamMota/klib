#include "klinklist.h"
#include "kmalloc.h"
#include <stddef.h>
#include <stdlib.h>

KLinkedList *klinklist_new(void) {
  KLinkedList *list = KMALLOC(KLinkedList);
  list->head = NULL;
  list->len = 0;
  return list;
}

void klinklist_free(KLinkedList **list) {
  if (!list || !*list)
    return;
  KLinkedListNode *curr = (*list)->head;
  while (curr) {
    KLinkedListNode *next = curr->next;
    KFREE(curr);
    curr = next;
  }
  KFREE(*list);
  *list = NULL;
}

KLinkedListNode *klinklist_new_node(void *data) {
  KLinkedListNode *node = KMALLOC(KLinkedListNode);
  node->data = data;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

void klinklist_free_node(KLinkedListNode **node) {
  if (!node || !*node)
    return;
  (*node)->next = NULL;
  (*node)->prev = NULL;
  KFREE(*node);
  *node = NULL;
}

bool klinklist_append(KLinkedList *list, KLinkedListNode *node) {
  if (!list || !node)
    return false;
  if (!list->head) {
    list->head = node;
    node->prev = NULL;
    node->next = NULL;
    list->len++;
    return true;
  }
  KLinkedListNode *curr = list->head;
  while (curr->next)
    curr = curr->next;
  curr->next = node;
  node->prev = curr;
  node->next = NULL;
  list->len++;
  return true;
}

bool klinklist_prepend(KLinkedList *list, KLinkedListNode *node) {
  if (!list || !node)
    return false;
  node->prev = NULL;
  node->next = list->head;
  if (list->head)
    list->head->prev = node;
  list->head = node;
  list->len++;
  return true;
}

bool klinklist_remove(KLinkedList *list, KLinkedListNode *node) {
  if (!list || !node)
    return false;
  if (node->prev)
    node->prev->next = node->next;
  else
    list->head = node->next;
  if (node->next)
    node->next->prev = node->prev;
  node->next = NULL;
  node->prev = NULL;
  list->len--;
  return true;
}

/* Remove e retorna o último nó (sem liberar). */
KLinkedListNode *klinklist_pop(KLinkedList *list) {
  if (!list || !list->head)
    return NULL;
  KLinkedListNode *curr = list->head;
  while (curr->next)
    curr = curr->next;
  klinklist_remove(list, curr);
  return curr;
}

/* Remove e retorna o primeiro nó (sem liberar). */
KLinkedListNode *klinklist_pop_front(KLinkedList *list) {
  if (!list || !list->head)
    return NULL;
  KLinkedListNode *node = list->head;
  klinklist_remove(list, node);
  return node;
}

KLinkedListNode *klinklist_get(KLinkedList *list, size_t index) {
  if (!list || index >= list->len)
    return NULL;
  KLinkedListNode *curr = list->head;
  for (size_t i = 0; i < index; i++)
    curr = curr->next;
  return curr;
}

KLinkedListNode *klinklist_first(KLinkedList *list) {
  if (!list)
    return NULL;
  return list->head;
}

KLinkedListNode *klinklist_last(KLinkedList *list) {
  if (!list || !list->head)
    return NULL;
  KLinkedListNode *curr = list->head;
  while (curr->next)
    curr = curr->next;
  return curr;
}

size_t klinklist_len(KLinkedList *list) {
  if (!list)
    return 0;
  return list->len;
}

bool klinklist_empty(KLinkedList *list) {
  if (!list)
    return true;
  return list->len == 0;
}

KLinkedListNode *klinklist_find(KLinkedList *list, void *data) {
  if (!list)
    return NULL;
  KLinkedListNode *curr = list->head;
  while (curr) {
    if (curr->data == data)
      return curr;
    curr = curr->next;
  }
  return NULL;
}

bool klinklist_contains(KLinkedList *list, void *data) {
  return klinklist_find(list, data) != NULL;
}

void klinklist_clear(KLinkedList *list) {
  if (!list)
    return;
  KLinkedListNode *curr = list->head;
  while (curr) {
    KLinkedListNode *next = curr->next;
    KFREE(curr);
    curr = next;
  }
  list->head = NULL;
  list->len = 0;
}

void klinklist_reverse(KLinkedList *list) {
  if (!list || list->len <= 1)
    return;
  KLinkedListNode *curr = list->head;
  KLinkedListNode *tmp = NULL;
  while (curr) {
    tmp = curr->prev;
    curr->prev = curr->next;
    curr->next = tmp;
    curr = curr->prev; /* antigo next */
  }
  if (tmp)
    list->head = tmp->prev;
}
