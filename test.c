#include "stdio.h"
#include "stdlib.h"
#include "test.h"
node* creatList(void) {
    node* list = (node*)malloc(sizeof(node));
    if (list == NULL)return NULL;
    list->data = 0;
    list->next = NULL;
    return list;
}
int insert_node(node* list_head, int i, int data) {
    int j = 1;
    node* listhead = list_head;
    node* p = (node*)malloc(sizeof(node));
    if (p == NULL)return 0;
    while (listhead && j < i) {
        listhead = listhead->next;
        j++;
    }
    if (!listhead || j > i) {
        free(p);
        return 0;
    }
    p->data = data;
    p->next = listhead->next;
    listhead->next = p;
    return 1;
}

int delete_node(node* list_head, int data) {
    node* listhead = list_head;
    node* p;
    while (listhead->next) {
        if (listhead->next->data == data) {
            p = listhead->next;
            listhead->next= p->next;
            free(p);
            return 1;
        }
        else {
            listhead = listhead->next;
        }
    }
    return 0;
}

void print_list(node* list_head) {
    node* listhead = list_head;
    if (listhead == NULL)return;
    node* p = listhead->next;
    while (p) {
        printf("%d,", p->data);
        p = p->next;
    }
}
