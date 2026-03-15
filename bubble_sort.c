#include "stdio.h"
#include "bubble_sort.h"
void swap(sq_list* sq, int i, int j) {
	int temp = sq->data[i];
	sq->data[i] = sq->data[j];
	sq->data[j] = temp;
}
void bubble_sort(sq_list* sq) {
	int flag = 1;
	for (int i = 0; i < sq->length&&flag; i++) {
		flag = 0;
		for (int j = sq->length-2; j >= i; j--) {
			if (sq->data[j+1] < sq->data[j]) {
				swap(sq, j, j + 1);
				flag = 1;
			}
		}
	}
}
void print_sq_list(sq_list* sq) {
	if (sq == NULL || sq->length == 0) {
		printf("Ë³Ðò±íÎª¿Õ\n");
		return;
	}
	for (int i = 0; i < sq->length; i++) {
		printf("%d ", sq->data[i]);
	}
	printf("\n");
}