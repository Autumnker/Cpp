/*===============================================
*   文件名称：linkList.hpp
*   创 建 者：hqyj
*   创建日期：2025年07月30日
*   描    述：定义命名空间和声明变量、函数
================================================*/
#ifndef _linkList_H
#define _linkList_H
#ifdef __cplusplus
extern "C"
{
#endif

    /**命名空间 */
    namespace list_LK
    {
        typedef int val_t; // 自定义数据类型
        struct linkList;   // 结构体

        /**
         * @brief 验证val_t类型的数据是否错误
         * @param val val_t类型的参数
         * @return bool 如果val错误,返回true;如果val正确,返回false
         */
        bool val_tIsError(val_t val);

        /**
         * @brief 创建一个节点
         * @param val 初始化节点的值
         * @return struct link_LK::linkList* 结构体指针
         */
        struct linkList *createNode(list_LK::val_t val);

        /**
         * @brief 删除一个节点
         * @param node 节点指针
         * @return bool 成功返回true,失败返回false
         */
        bool deleteNode(struct linkList *&node);

        /**
         * @brief 头插法
         * @param head 指向表头指针
         * @param val 要插入的值
         * @return bool 成功返回true,失败返回false
         */
        bool insertAtHead(struct linkList *&head, val_t val);

        /**
         * @brief 根据索引修改链表节点的值
         * @param head 头节点指针
         * @param index 待修改节点的索引
         * @param val 新的值
         * @return bool 成功返回true,失败返回false
         */
        bool changeValByIndex(struct linkList *head, int index, val_t val);

        /**
         * @brief 根据索引删除节点
         * @param head 头节点指针
         * @param index 待删除节点的索引
         * @return bool 成功返回true,失败返回false
         */
        bool deleteByIndex(struct linkList *&head, int index);

        /**
         * @brief 销毁链表
         * @param head 指向表头的指针
         * @return bool 成功返回true,失败返回false
         */
        bool destoryList(struct linkList *&head);

        /**
         * @brief 打印链表
         * @param head 指向表头的指针
         * @return void
         */
        void printList(struct linkList *head);
    }

    /**结构体定义 */
    typedef struct list_LK::linkList
    {
    private:
        struct list_LK::linkList *_next;
        list_LK::val_t _val;

    public:
        bool setNext(struct list_LK::linkList *next);
        struct list_LK::linkList *getNext();
        bool setVal(list_LK::val_t val);
        list_LK::val_t getVal();

    } linkList;

#ifdef __cplusplus
}
#endif
#endif // linkList_H
