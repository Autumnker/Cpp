/*===============================================
*   文件名称：main..c
*   创 建 者：Autumnker
*   创建日期：2025年08月04日
*   描    述：
================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./threadPool.h"

void Fun(void* arg) {
    int index = *(int*)arg;
    printf("Fun: index = %d\n", index);
    usleep(500000);
    free(arg);
}

int main(int argc, char* argv[]) {
    ThreadPool* pool = initThreadPool(100, 5, 150);
    Task task;

    // 测试线程增加机制
    for (int i = 0; i < 1300; ++i) {
        int* index = (int*)malloc(sizeof(int));
        *index = i;
        task.arg = index;
        task.fun = Fun;
        addTask(task, pool);
    }

    // 测试线程销毁机制
    for (int i = 0; i < 500; ++i) {
        int* index = (int*)malloc(sizeof(int));
        *index = i;
        task.arg = index;
        task.fun = Fun;
        addTask(task, pool);
        usleep(50000);
    }

    sleep(3);
    destoryThreadPool(pool);

    return 0;
}
