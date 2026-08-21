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
  KNode *curr = (*list)->head;
  while (curr) {
    KNode *next = curr->next;
    KFREE(curr);
    curr = next;
  }
  KFREE(*list);
  *list = NULL;
}

KNode *klinklist_new_node(void *data) {
  KNode *node = KMALLOC(KNode);
  node->data = data;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

void klinklist_free_node(KNode **node) {
  if (!node || !*node)
    return;
  (*node)->next = NULL;
  (*node)->prev = NULL;
  KFREE(*node);
  *node = NULL;
}

bool klinklist_append(KLinkedList *list, KNode *node) {
  if (!list || !node)
    return false;
  if (!list->head) {
    list->head = node;
    node->prev = NULL;
    node->next = NULL;
    list->len++;
    return true;
  }
  KNode *curr = list->head;
  while (curr->next)
    curr = curr->next;
  curr->next = node;
  node->prev = curr;
  node->next = NULL;
  list->len++;
  return true;
}

bool klinklist_prepend(KLinkedList *list, KNode *node) {
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

bool klinklist_remove(KLinkedList *list, KNode *node) {
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
KNode *klinklist_pop(KLinkedList *list) {
  if (!list || !list->head)
    return NULL;
  KNode *curr = list->head;
  while (curr->next)
    curr = curr->next;
  klinklist_remove(list, curr);
  return curr;
}

/* Remove e retorna o primeiro nó (sem liberar). */
KNode *klinklist_pop_front(KLinkedList *list) {
  if (!list || !list->head)
    return NULL;
  KNode *node = list->head;
  klinklist_remove(list, node);
  return node;
}

KNode *klinklist_get(KLinkedList *list, size_t index) {
  if (!list || index >= list->len)
    return NULL;
  KNode *curr = list->head;
  for (size_t i = 0; i < index; i++)
    curr = curr->next;
  return curr;
}

KNode *klinklist_first(KLinkedList *list) {
  if (!list)
    return NULL;
  return list->head;
}

KNode *klinklist_last(KLinkedList *list) {
  if (!list || !list->head)
    return NULL;
  KNode *curr = list->head;
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

KNode *klinklist_find(KLinkedList *list, void *data) {
  if (!list)
    return NULL;
  KNode *curr = list->head;
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
  KNode *curr = list->head;
  while (curr) {
    KNode *next = curr->next;
    KFREE(curr);
    curr = next;
  }
  list->head = NULL;
  list->len = 0;
}

void klinklist_reverse(KLinkedList *list) {
  if (!list || list->len <= 1)
    return;
  KNode *curr = list->head;
  KNode *tmp = NULL;
  while (curr) {
    tmp = curr->prev;
    curr->prev = curr->next;
    curr->next = tmp;
    curr = curr->prev; /* antigo next */
  }
  if (tmp)
    list->head = tmp->prev;
}
