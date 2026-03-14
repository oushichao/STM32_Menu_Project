#ifndef TEST_H
#define TEST_H
#include "stdio.h"
typedef struct node {
    int data;
    struct node* next;
} node;
node* creatList(void);
int insert_node(node* list_head, int i, int data);
int delete_node(node* list_head, int data);
void print_list(node* list_head);
#endif