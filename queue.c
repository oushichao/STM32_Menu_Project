#include "stdio.h"
#include "queue.h"
queue create_queue() {
	queue q;
	q.front = 0;
	q.rear = 0;
	return q;
}

int enqueue(queue* q, int val) {
	if (q == NULL)return 0;
	if ((q->rear + 1) % MAXSIZE == q->front)return 0;
	q->data[q->rear] = val;
	q->rear = (q->rear + 1) % MAXSIZE;
	return 1;
}
int dequeue(queue* q) {
	if (q == NULL)return 0;
	if (q->front == q->rear)return 0;
	q->front = (q->front + 1) % MAXSIZE;
	return 1;
}
int get_front(queue* q,int *out_val) {
	if (q == NULL|| q->front == q->rear)return 0;
	*out_val = q->data[q->front];
	return 1;
}
void print_queue(queue* q) {
	if (q == NULL)return ;
	if (q->rear > q->front) {
		for (int i = q->front; i < q->rear; i++)
			printf("%d,", q->data[i]);
	}
	else if (q->rear < q->front) {
		for (int i = q->front; i < MAXSIZE; i++)
			printf("%d,", q->data[i]);
		for (int j = 0; j < q->rear; j++)
			printf("%d,",q->data[j]);
	}
	else if (q->rear == q->front) {
		printf("╤сапн╙©у\n");
	}
}