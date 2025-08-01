/*===============================================
*   文件名称：main.cpp
*   创 建 者：Autumnker
*   创建日期：2025年07月31日
*   描    述：
================================================*/
#include <iostream>
#include <string.h>
#include "./MyString.hpp"
using namespace std;

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    // 创建字符串
    char input[BUFF_SIZE] = {'\0'};
    cout << "请输入字符串：";
    cin >> input;

    mstr::MyStr *my_str = mstr::MyStr::createStr(input);
    my_str->printStr();

    // 复制字符串
    cout << "以下为字符串拷贝之前的对象信息" << endl;
    mstr::MyStr copy_str;
    copy_str.printStr();
    cout << "以下为字符串拷贝之后的对象信息" << endl;
    mstr::MyStr::copyStr(*my_str, copy_str);
    copy_str.printStr();

    // 追加字符串
    cout << "请输入需要追加的字符：" << endl;
    memset(input, '\0', BUFF_SIZE);
    cin >> input;
    my_str->pushBack(input);
    my_str->printStr();

    // 在索引处插入字符串
    int index = -1;
    memset(input, '\0', BUFF_SIZE);
    cout << "请输入索引：" << endl;
    cin >> index;
    cout << "请输入插入内容：" << endl;
    cin >> input;
    my_str->insertByIndex(input, index);
    my_str->printStr();

    // 释放资源
    delete my_str;

    return 0;
}
