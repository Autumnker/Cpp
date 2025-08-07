/*===============================================
*   文件名称：main.cpp
*   创 建 者：Autumnker
*   创建日期：2025年08月06日
*   描    述：
================================================*/
#include <iostream>
#include "./linkListTemplate.h"

using namespace std;

int main(int argc, char* argv[])
{
    LinkList<int> list;

    for (int i = 0; i < 10; ++i) {
        list.insertFront(i + 'a');
        // list.pushBack(i + 1);
    }
    cout << "lsit: " << list;

    LinkList<int> list2;
    list2 = -list;
    cout << "list2: " << list2;
    cout << "list: " << list;

    LinkList<int> list3;
    cout << "list3: " << list3;
    list3 = list2;
    cout << "list3: " << list3;
    list3 = -list3;
    cout << "list3: " << list3;

    list3 = list2 + list;
    cout << "list3: " << list3;

    list3 = list3 + list3;
    cout << "list3: " << list3;


    return 0;
}
