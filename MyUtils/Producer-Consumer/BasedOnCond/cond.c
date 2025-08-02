#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

// 数据结构
typedef struct Node {
    int val;
    struct Node* next;
} Node;

Node* head = NULL;

// 工具函数
void printList(Node* head, int flag) {
    char* addLine = "++++++++++++++++++++++++++++++++++++++++++";
    char* subLine = "------------------------------------------";
    if (flag == 0) {
        printf("%s\n", addLine);
    } else {
        printf("%s\n", subLine);
    }

    Node* curr = head;
    while (curr != NULL) {
        if (curr->next != NULL) {
            printf("%d -> ", curr->val);
        } else {
            printf("%d\n", curr->val);
        }
        curr = curr->next;
    }

    if (flag == 0) {
        printf("%s\n", addLine);
    } else {
        printf("%s\n", subLine);
    }
}

// 生产者
void* producer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // 创建节点
        Node* node = (Node*)malloc(sizeof(Node));
        node->val = rand() % 100;
        node->next = head;

        // 更新头节点
        head = node;

        pthread_mutex_unlock(&mutex);

        // 打印当前链表
        printList(head, 0);

        // 唤醒条件变量
        pthread_cond_broadcast(&cond);

        sleep(1);
    }
    pthread_exit(0);
}

// 消费者
void* consumer(void* arg) {
    Node* node = NULL;
    while (1) {
        pthread_mutex_lock(&mutex);

        // 消费完毕后阻塞等待生产者继续生产
        while (head == NULL) {
            pthread_cond_wait(&cond, &mutex);
        }

        // 消费
        node = head;
        head = head->next;
        free(node);

        pthread_mutex_unlock(&mutex);

        // 打印链表
        printList(head, 1);

        usleep(400000);
    }
    pthread_exit(0);
}

#define PTHREAD_NUM 5
int main(int argc, char const* argv[]) {
    // 创建数组
    pthread_t p[PTHREAD_NUM];
    pthread_t c[PTHREAD_NUM];
    int producer_num = 0;
    int consumer_num = 0;

    for (int i = 0; i < PTHREAD_NUM; ++i) {
        if (0 != pthread_create(&p[i], NULL, producer, NULL)) {
            printf("pthread_create p: error\n");
            break;
        }
        producer_num++;
    }
    for (int i = 0; i < PTHREAD_NUM / 2; ++i) {
        if (0 != pthread_create(&c[i], NULL, consumer, NULL)) {
            printf("pthread_create c: error\n");
            break;
        }
        consumer_num++;
    }

    for (int i = 0; i < producer_num; ++i) {
        pthread_join(p[i], NULL);
    }
    for (int i = 0; i < consumer_num; ++i) {
        pthread_join(c[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}