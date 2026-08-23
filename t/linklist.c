#include "types/klinklist.h"
#include <stdio.h>

int main(void) {
    KLinkedList *list = klinklist_new();

    if (!list) {
        printf("Erro ao criar lista\n");
        return 1;
    }

    int a = 10;
    int b = 20;
    int c = 30;

    KLinkedListNode *node1 = klinklist_new_node(&a);
    KLinkedListNode *node2 = klinklist_new_node(&b);
    KLinkedListNode *node3 = klinklist_new_node(&c);

    klinklist_append(list, node1);
    klinklist_append(list, node2);
    klinklist_append(list, node3);

    KLinkedListNode *curr = list->head;

    while (curr) {
        printf("%d\n", *(int *)curr->data);
        curr = curr->next;
    }

    printf("Tamanho: %zu\n", list->len);
    klinklist_free(&list);

    return 0;
}
