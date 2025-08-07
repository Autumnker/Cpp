/*===============================================
*   文件名称：linkListTemplate.h
*   创 建 者：Autumnker
*   创建日期：2025年08月06日
*   描    述：
================================================*/
#ifndef _LINKLISTTEMPLATE_H
#define _LINKLISTTEMPLATE_H

#include <iostream>

using namespace std;

/*节点类型*/
template <typename T> struct Node {
    T _data;
    struct Node<T>* _next;
};

/*链表类*/
template <typename T> class LinkList {
public:
    LinkList() {
        // 创建头节点
        _head = new Node<T>;
        _len = 0;
        _head->_next = nullptr;
        _tail = nullptr;
    }

    LinkList(const LinkList<T>& list) {
        this->deepCopy(list);
    }

    ~LinkList() {
        Node<T>* currNode = _head;
        Node<T>* nextNode = currNode->_next;
        while (currNode != nullptr) {
            delete currNode;
            currNode = nextNode;
            if (nextNode != nullptr) {
                nextNode = nextNode->_next;
            }
        }
        _head = nullptr;
        _tail = nullptr;
    }

    LinkList<T>& operator=(const LinkList<T>& list) {
        // 销毁原有内容
        this->destoryList();
        // 开始深拷贝
        deepCopy(list);
        return *this;
    }

    void destoryList() {
        Node<T>* currNode = _head;
        Node<T>* nextNode = currNode->_next;
        while (currNode != nullptr) {
            delete currNode;
            currNode = nextNode;
            if (nextNode != nullptr) {
                nextNode = nextNode->_next;
            }
        }
        _head = nullptr;
        _tail = nullptr;
        _len = 0;
    }

    void deepCopy(const LinkList<T>& list) {
        // 创建头节点
        _head = new Node<T>;
        _len = 0;
        _head->_next = nullptr;
        // 获取资源节点及资源链表节点数量
        Node<T>* srcNode = list._head->_next;
        int srcNum = list._len;
        // 拷贝
        Node<T>* preNode = _head;
        for (int i = 0; i < srcNum; ++i) {
            // 创建新节点
            Node<T>* currNode = new Node<T>;
            // 赋值
            currNode->_data = srcNode->_data;
            // 上一个节点指向当前节点
            preNode->_next = currNode;
            // 资源节点后移,上一个节点后移
            srcNode = srcNode->_next;
            preNode = preNode->_next;
            // 节点计数+1
            _len++;
        }
        _tail = preNode;
    }

    void insertFront(const T& data) {
        Node<T>* node = new Node<T>;
        node->_data = data;
        node->_next = _head->_next;
        _head->_next = node;
        _len++;     // 头节点_data字段记录节点数量（不算头节点）

        if (_len == 1) {    // 尾节点指向第一个从头部插入的节点
            _tail = _head->_next;
        }
    }

    void pushBack(const T& data) {
        Node<T>* node = new Node<T>;
        node->_data = data;
        node->_next = nullptr;
        if (_len == 0) {    // 如果是尾插法插入第一个节点
            _head->_next = node;
            _tail = node;
            _len++;
            return;
        }
        _tail->_next = node;
        _tail = node;
        _len++;
    }

    LinkList<T> operator-() {
        // 遍历旧链表并用头插法创建一个新链表
        LinkList<T> list;  // 会调用无参构造函数,自定义/默认
        Node<T>* srcNode = _head->_next;
        int num = _len;
        for (int i = 0; i < num; ++i) {
            list.insertFront(srcNode->_data);
            srcNode = srcNode->_next;
        }

        // // 销毁现有的链表
        // this->destoryList();

        // // 重新赋值
        // _head = list._head;
        // _tail = list._tail;
        return list;
    }


    /*友元函数*/
    template <typename T1>
    friend std::ostream& operator<<(std::ostream& out, const LinkList<T1>& list);
    template <typename T2>
    friend LinkList<T2> operator+(const LinkList<T2>& listA, const LinkList<T2>& listB);

private:
    Node<T>* _head;
    Node<T>* _tail;
    int _len;
};

template <typename T> std::ostream& operator<<(std::ostream& out, const LinkList<T>& list) {
    Node<T>* currNode = list._head->_next;
    int num = list._len;
    for (int i = 0; i < num; ++i) {
        out << currNode->_data;
        currNode = currNode->_next;
        if (i != num - 1) {
            cout << " -> ";
        }
    }
    out << endl;
    return out;
}

template <typename T> LinkList<T> operator+(const LinkList<T>& listA, const LinkList<T>& listB) {
    LinkList<T> list;
    Node<T>* srcNode = listA._head->_next;
    for (int i = 0; i < listA._len; ++i) {
        list.pushBack(srcNode->_data);
        srcNode = srcNode->_next;
    }

    srcNode = listB._head->_next;
    for (int i = 0; i < listB._len; ++i) {
        list.pushBack(srcNode->_data);
        srcNode = srcNode->_next;
    }

    return list;
}

#endif
