/*===============================================
*   文件名称：main.c
*   创 建 者：Autumnker
*   创建日期：2025年08月02日
*   描    述：
================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./threadPool.h"

void Fun(void* arg) {
    int index = *(int*)arg;
    printf("Fun函数执行 index = %d\n", index);
    free(arg);
}

typedef struct {
    char name[25];
    char address[25];
    int age;
} ARG;

void Fun2(void* arg) {
    ARG* _arg = (ARG*)arg;
    printf("Fun2:name = %s, address = %s, age = %d\n", _arg->name, _arg->address, _arg->age);
    free(arg);
}

int main(int argc, char* argv[]) {
    // 创建线程池
    ThreadPool* pool = initThreadPool(20, 10, 50);
    Task task = {0};

    // 循环赋值给task
    for (int i = 0; i < 100; ++i) {
        printf("index = %d\n", i);
        // 添加task
        int* p = (int*)malloc(sizeof(int));
        *p = i;
        task.fun = Fun;
        task.arg = p;
        addTask(pool, task);

        ARG* arg = (ARG*)malloc(sizeof(ARG));
        strcpy(arg->name, "小明");
        strcpy(arg->address, "成华大道");
        arg->age = i + 1;

        task.fun = Fun2;
        task.arg = arg;
        addTask(pool, task);
    }

    // sleep(3);

    // 销毁线程池
    destoryThreadPool(pool);

    return 0;
}
