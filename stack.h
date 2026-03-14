#pragma once
#include"stdio.h"
#define MAXSIZE 64
typedef struct {
	int top;
	int data[MAXSIZE];
}Stack;
Stack creat_stack();
int push(Stack* s, int val);
int pop(Stack* s);
int get_top(Stack* s,int *num);
void print_stack(Stack* s);