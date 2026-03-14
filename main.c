#include "stdio.h"
#include "queue.h"

int main() {
    // 1. 创建空队列
    queue q = create_queue();
    printf("=== 初始化队列 ===\n");
    printf("初始队列：");
    print_queue(&q);
    printf("\n\n");

    // 2. 入队3个值：10、20、30
    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);
    printf("=== 入队10、20、30后 ===\n");
    printf("队列内容：");
    print_queue(&q);
    printf("\n");

    // 3. 取队头值
    int front_val;
    get_front(&q, &front_val);
    printf("当前队头值：%d\n\n", front_val);

    // 4. 出队1次（弹出10）
    dequeue(&q);
    printf("=== 出队1次后 ===\n");
    printf("队列内容：");
    print_queue(&q);
    printf("\n");

    return 0;
}