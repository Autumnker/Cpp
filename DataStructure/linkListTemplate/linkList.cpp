/*===============================================
*   文件名称：linkList.cpp
*   创 建 者：hqyj
*   创建日期：2025年07月30日
*   描    述：
1.  封装一个结构体链表
2. 实现创建、增、删、打印
================================================*/
#include <iostream>
#include "./linkList.hpp"

using namespace std;

/**实现结构体函数 */
bool linkList::setNext(linkList *next)
{
    _next = next;
    return true;
}
linkList *linkList::getNext()
{
    return _next;
}
bool linkList::setVal(list_LK::val_t val)
{
    _val = val;
    return true;
}
list_LK::val_t linkList::getVal()
{
    return _val;
}

/**定义函数 */
bool list_LK::val_tIsError(list_LK::val_t val)
{
    return false; // 在int形链表中不存在错误的类型
}
linkList *list_LK::createNode(list_LK::val_t val)
{
    linkList *node = new linkList;

    if (node == nullptr)
    {
        printf("list_LK::linkList::createNode: node is a nullptr\n");
        return nullptr;
    }

    if (!node->setNext(nullptr) || !node->setVal(val))
    {
        printf("list_LK::linkList::createNode: set error\n");
        return nullptr;
    }

    return node;
}
bool list_LK::deleteNode(linkList *&node)
{
    if (node == nullptr)
    {
        printf("linkList::deleteNode:node is a nullptr\n");
        return false;
    }
    delete node;
    node = nullptr;
    return true;
}
bool list_LK::insertAtHead(linkList *&head, list_LK::val_t val)
{
    if (head == nullptr || list_LK::val_tIsError(val))
    {
        printf("list_LK::insertAtHead:参数错误\n");
        return false;
    }

    // 创建节点
    linkList *newNode = list_LK::createNode(val);
    newNode->setNext(head);
    head = newNode;
    return true;
}

bool list_LK::changeValByIndex(linkList *head, int index, list_LK::val_t val)
{
    if (head == nullptr || index < 0 || val_tIsError(val))
    {
        printf("list_LK::changeValByIndex: args error\n");
        return false;
    }

    linkList *currNode = head;
    for (int i = 0; i < index; ++i)
    {
        if (currNode->getNext() == nullptr)
        {
            printf("list_LK::changeValByIndex: index > the max index of list\n");
            return false;
        }
        currNode = currNode->getNext();
    }

    currNode->setVal(val);
    return true;
}

bool list_LK::deleteByIndex(linkList *&head, int index)
{
    if (head == nullptr || index < 0)
    {
        printf("list_LK::deleteByIndex:参数错误\n");
        return false;
    }

    linkList *preNode = nullptr;
    linkList *currNode = head;

    // 如果要删除头节点
    if (index == 0)
    {
        head = head->getNext();
        list_LK::deleteNode(currNode);
        return true;
    }

    // 找到待删除的节点
    for (int i = 0; i < index; ++i)
    {
        if (currNode->getNext() == nullptr)
        {
            printf("list_LK::deleteByIndex: index > the max index of list\n");
            return false;
        }

        preNode = currNode;
        currNode = currNode->getNext();
    }

    preNode->setNext(currNode->getNext());
    list_LK::deleteNode(currNode);
    return true;
}

bool list_LK::destoryList(linkList *&head)
{
    if (head == nullptr)
    {
        printf("list_LK::destoryList: arg is a nullptr\n");
        return false;
    }

    linkList *nextNode = head->getNext();
    while (head != nullptr)
    {
        delete head;
        head = nextNode;
        if (nextNode != nullptr)
        {
            nextNode = nextNode->getNext();
        }
    }

    return true;
}

void list_LK::printList(linkList *head)
{
    if (head == nullptr)
    {
        printf("list_LK::printList: arg is a nullptr\n");
        return;
    }

    linkList *currNode = head;
    while (currNode != nullptr)
    {
        if (currNode->getNext() != nullptr)
        {
            printf("%d -> ", currNode->getVal());
        }
        else
        {
            printf("%d\n", currNode->getVal());
        }
        currNode = currNode->getNext();
    }
}
