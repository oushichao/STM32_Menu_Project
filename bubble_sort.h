#pragma once
#include "stdio.h"
#define MAXSIZE 64
typedef struct {
	int data[MAXSIZE];
	int length;
}sq_list;
void swap(sq_list* sq, int i, int j);
void bubble_sort(sq_list* sq);
void print_sq_list(sq_list* sq);
