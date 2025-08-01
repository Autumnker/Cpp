/*===============================================
*   文件名称：main.cpp
*   创 建 者：hqyj
*   创建日期：2025年07月30日
*   描    述：
================================================*/
#include <iostream>
#include "./linkList.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    int num = 0;
    int index = -1;
    int val = 0;

    // 创建头指针
    linkList *head = nullptr;

    // 创建链表
    cout << "请输入链表长度：";
    cin >> num;

    for (int i = 1; i <= num; ++i)
    {
        if (i == 1)
        {
            head = list_LK::createNode(i);
            list_LK::printList(head);
        }
        else
        {
            list_LK::insertAtHead(head, i);
            list_LK::printList(head);
        }
    }

    // 修改链表
    cout << "请输入要修改的索引:";
    cin >> index;
    cout << "请输入新的值：";
    cin >> val;
    list_LK::changeValByIndex(head, index, val);
    list_LK::printList(head);

    // 删除节点
    cout << "请输入要删除节点的索引：";
    cin >> index;
    list_LK::deleteByIndex(head, index);
    list_LK::printList(head);

    // 删除链表
    list_LK::destoryList(head);

    // 测试链表是否已经被全部删除
    list_LK::printList(head);

    return 0;
}
