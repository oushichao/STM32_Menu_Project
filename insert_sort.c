#include "stdio.h"
#include "insert_sort.h"
void insert_sort(sq_list*sq) {
	if (sq == NULL || sq->length <= 1)return;
	int j;
	for (int i = 1; i < sq->length; i++) {
		if (sq->data[i - 1] > sq->data[i]) {
			int temp = sq->data[i];
			for (j = i - 1; j >= 0 && sq->data[j] > temp; j--)
				sq->data[j+1] = sq->data[j];
			sq->data[j+1] = temp;
		}
	}
}