/*===============================================
*   文件名称：MyString.cpp
*   创 建 者：Autumnker
*   创建日期：2025年07月31日
*   描    述：
================================================*/
#include <stdio.h>
#include <string.h>

#include "./MyString.hpp"
using namespace mstr;

void MyStr::setLen(int len)
{
    _len = len;
}
int MyStr::getLen() const
{
    return _len;
}
void MyStr::setHead(char *head)
{
    _head = head;
}
char *MyStr::getHead() const
{
    return _head;
}

MyStr::~MyStr()
{
    printf("%p 正在被析构,原长度为%d    ", _head, _len);

    delete[] _head;
    _head = nullptr;
    _len = 0;

    printf("析构完成\n");
}

MyStr *MyStr::createStr(const char *string)
{
    int len = strlen(string);
    if (len <= 0)
    {
        printf("MyStr::createStr:string is empty\n");
        return nullptr;
    }
    char *head = new char[len + 1];

    strcpy(head, string); // 深拷贝

    MyStr *my_str = new MyStr(len, head);
    return my_str;
}

void MyStr::printStr()
{
    if (_head != nullptr && _len > 0)
    {
        printf("%s\n", _head);
    }
    else
    {
        printf("null\n");
    }
}

bool MyStr::copyStr(const MyStr &src_str, MyStr &dest_str)
{
    if (src_str.getLen() <= 0)
    {
        printf("MyStr::copyStr:src_str is empty\n");
        return false;
    }
    int len = src_str.getLen();
    char *head = new char[len + 1];
    strcpy(head, src_str.getHead());
    dest_str.setLen(len);
    dest_str.setHead(head);
    return true;
}

bool MyStr::pushBack(const char *subStr)
{
    if (_len < 0 || subStr == NULL || strlen(subStr) <= 0)
    {
        printf("MyStr::pushBack: args error\n");
        return false;
    }

    int new_len = _len + strlen(subStr);

    char *new_head = new char[new_len + 1];
    memset(new_head, '\0', new_len + 1);
    strcpy(new_head, _head);
    strcat(new_head, subStr);
    // 释放旧空间
    delete[] _head;
    // 赋予新空间
    this->setLen(new_len);
    this->setHead(new_head);

    return true;
}

bool MyStr::insertByIndex(const char *subString, int index)
{
    if (_len < 0 || subString == NULL || strlen(subString) <= 0)
    {
        printf("MyStr::insertByIndex:aim_str or subString error\n");
        return false;
    }
    if (index < 0 || index > _len)
    {
        printf("MyStr::insertByIndex: index error\n");
        return false;
    }

    // 开辟空间
    int new_len = _len + strlen(subString);
    char *new_head = new char[new_len + 1];
    memset(new_head, '\0', new_len + 1);
    // 复制字符串
    strncpy(new_head, _head, index);
    strcat(new_head, subString);
    strcat(new_head, _head + index);
    // 释放旧空间
    delete[] _head;
    // 成员变量赋值
    this->setHead(new_head);
    this->setLen(new_len);

    return true;
}

/*以后有空再实现下面的函数*/

bool MyStr::replaceStr(MyStr &my_str, const char *srcString, const char *destString) { return true; }

int MyStr::getLength(const MyStr &my_str) { return 0; }

bool MyStr::isEmpty(const MyStr &my_str) { return true; }

char *MyStr::findFirstSubStr(const MyStr &my_str, const char *subStr) { return nullptr; }

char MyStr::getCharByIndex(const MyStr &my_str, int index) { return '\0'; }

bool MyStr::clearStr(MyStr &my_str) { return true; }

bool MyStr::swapStr(MyStr &my_strA, MyStr &my_strB) { return true; }

int MyStr::compareStr(const MyStr &my_strA, const MyStr &my_strB) { return 0; }

std::queue<char *> MyStr::findAllSubStr(const MyStr &my_str, const char *subStr) { return std::queue<char *>(); }
