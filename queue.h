#include "stdio.h"
#pragma once
#define MAXSIZE 64
typedef struct {
	int rear;
	int front;
	int data[MAXSIZE];
}queue;
queue create_queue();
int enqueue(queue* q, int val);
int dequeue(queue* q);
int get_front(queue* q, int* out_val);
void print_queue(queue* q);