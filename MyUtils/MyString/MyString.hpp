/*===============================================
*   文件名称：MyString.hpp
*   创 建 者：Autumnker
*   创建日期：2025年07月31日
*   描    述：
1.  构造函数
2.  析构函数
3.  深拷贝/浅拷贝
4.  成员变量 int len; char *p;
5.  成员函数(功能描述)
    5.1 赋值
    5.2 拷贝(创建一个独立于原数据的副本)
    5.3 查找子串首地址
    5.4 填充(拼接、插入)
    5.5 获取长度
    5.6 判断是否为空
    5.7 下标访问，带边界
    5.8 清空字符串(包含内存的释放)
    5.9 替换字符串(用字符串替换一个子串)
    5.10    交换两个字符串
    5.11    比较字符串长度
    5.12    返回所有子串的首地址
================================================*/
#ifndef _MYSTRING_H
#define _MYSTRING_H

#include <queue>

    namespace mstr
    {
        // 声明类
        /**
         * @brief 自定义字符串类
         * @param _head 字符串首地址(带有结束符的字符串)
         * @param _len 字符串长度(不包含结束符)
         */
        class MyStr
        {
        public:
            // MyStr() {};
            MyStr(int len = 0, char *head = nullptr) : _len(len), _head(head) {};
            ~MyStr();

            /**
             * @brief 打印字符串
             */
            void printStr();

            /**
             * @brief 创建字符串对象
             * @param string C风格字符串
             * @return MyStr 成功返回MyStr型变量的首地址，失败返回nullptr
             */
            static MyStr *createStr(const char *string);

            /**
             * @brief 创建一个字符串的副本
             * @param src_str 源字符串
             * @param dest_str 目标字符串
             * @return bool 成功返回true，失败返回false
             */
            static bool copyStr(const MyStr &src_str, MyStr &dest_str);

            /**
             * @brief 找到第一个字串的首地址
             * @param my_str 源字符串
             * @param subStr 子串
             * @return char* 成功返回首地址，失败返回nullptr
             */
            static char *findFirstSubStr(const MyStr &my_str, const char *subStr);

            /**
             * @brief 追加字符串
             * @param subStr 追加字符串
             * @return bool 成功返回true，失败返回false
             */
            bool pushBack(const char *subStr);

            /**
             * @brief 在索引位置插入字符串
             * @param subString 待插入字符串
             * @param index 插入位置索引
             * @return bool 成功返回true，失败返回false
             */
            bool insertByIndex(const char *subString, int index);

            /**
             * @brief 获取字符串长度
             * @param my_str 需要获取长度的字符串
             * @return int 成功返回字符串长度，失败返回 -1
             */
            int getLength(const MyStr &my_str);

            /**
             * @brief 判断字符串是否为空
             * @param my_str 需要判断是否为空的字符串
             * @return bool 是空返回true，非空返回false
             */
            bool isEmpty(const MyStr &my_str);

            /**
             * @brief 获取索引处的字符
             * @param my_str 字符所在字符串
             * @param index 想要查找的索引
             * @return char 成功返回字符失败返回 '\0'
             */
            char getCharByIndex(const MyStr &my_str, int index);

            /**
             * @brief 清空字符串
             * @param my_str 待清空的字符串
             * @return bool 成功返回true，失败返回false
             */
            bool clearStr(MyStr &my_str);

            /**
             * @brief 用一个字符串替换目标串中的子串
             * @param 待替换子串所在字符串
             * @param srcString 用于替换的字符串
             * @param destString 需要替换的子串
             * @return bool 成功返回true，失败返回false
             */
            bool replaceStr(MyStr &my_str, const char *srcString, const char *destString);

            /**
             * @brief 交换两个字符串
             * @param my_strA 用于交换的第一个字符串
             * @param my_strB 用于交换的第二个字符串
             * @return bool 成功返回true，失败返回false
             */
            bool swapStr(MyStr &my_strA, MyStr &my_strB);

            /**
             * @brief 比较两个字符串的长度差异(my_strA - my_strB 的长度)
             * @param my_strA 用于比较的字符串A
             * @param my_strB 用于比较的字符串B
             * @return int 返回 my_strA 的长度减去 my_strB的长度的结果
             */
            int compareStr(const MyStr &my_strA, const MyStr &my_strB);

            /**
             * @brief 以队列形式返回所有子串的首地址
             * @param my_str 用于查询的目标串
             * @param subStr 用于查询的子串
             * @return std::queue<char *> 返回所有子串的首地址
             */
            std::queue<char *> findAllSubStr(const MyStr &my_str, const char *subStr);

        private:
            void setLen(int len);
            int getLen() const;
            void setHead(char *head);
            char *getHead() const;

        private:
            char *_head;
            int _len;
        };
    }

#endif // MYSTRING_H
