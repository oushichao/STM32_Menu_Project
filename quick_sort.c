#include "stdio.h"
#include "quick_sort.h"
#include "bubble_sort.h"
void quick_sort(sq_list*sq) {
	if (sq == NULL||sq->length<=1)return;
	q_sort(sq, 0, sq->length-1);
}
void q_sort(sq_list* sq, int low, int high) {
	if(low<high) {
		int mid = partition(sq,low,high);
		q_sort(sq, low, mid-1);
		q_sort(sq, mid+1, high);
	}
}
int partition(sq_list* sq, int low, int high) {
	int num = sq->data[low];
	while (low < high) {
		while (low < high && num <= sq->data[high]) {
			high--;
		}
		swap(sq, low, high);
		while (low<high && num>=sq->data[low]) {
			low++;
		}
		swap(sq, low, high);
	}
	return low;
}
