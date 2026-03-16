#include "stdio.h"
#include "tool.h"
void swap(sq_list* sq, int i, int j) {
	int temp = sq->data[i];
	sq->data[i] = sq->data[j];
	sq->data[j] = temp;
};