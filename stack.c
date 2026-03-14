#include "stack.h"
#include "stdio.h"
Stack creat_stack() {
	Stack s;
	s.top = -1;
	return s;
}
int push(Stack* s, int val) {
	if (s == NULL)return 0;
	if (s->top >= MAXSIZE - 1)return 0;
	s->data[++s->top] = val;
	return 1;
}
int pop(Stack* s) {
	if (s == NULL)return 0;
	if (s->top <= -1)return 0;
	s->top--;
	return 1;
}
int get_top(Stack* s,int *out_val) {
	if (s == NULL)return 0;
	if (s->top <= -1)return 0;
	*out_val= s->data[s->top];
	return 1;
	
}
void print_stack(Stack* s) {
	if (s == NULL)return;
	for (int i = s->top; i >-1; i--) {
		printf("%d,", s->data[i]);
	}
}