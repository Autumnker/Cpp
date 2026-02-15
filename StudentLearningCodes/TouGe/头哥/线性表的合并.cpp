//#define _CRT_SECURE_NO_WARNINGS 1
//
///*合并链表*/
//#include <iostream>
//
//using namespace std;
//
//typedef struct node //定义节点数据结构
//{
//    int d;
//    struct node* next;
//}node;
//
//node* initnode()
//{
//    node* p = new node;
//    p->d = 0;
//    p->next = NULL;
//
//    return p;
//}
//
//void prtList(node* head) //负责输出显示线性链表
//{
//    cout << "当前链表的内容为：";
//    while (head != NULL)
//    {
//        cout << head->d << ' ';
//        head = head->next;
//    }
//    cout << endl;
//}
//
//node* combineList(node* AH, node* BH) //负责合并链表  
//{
//    node* ch = BH;
//
//    node* cur_a = AH;
//    node* tail_a = AH->next;
//    node* cur_b = BH;
//    node* tail_b = BH->next;
//
//    while (cur_a)
//    {
//        if (cur_b->next == NULL)
//        {
//            cur_b->next = cur_a;
//            break;
//        }
//
//        else if (cur_b->d >= cur_a->d && tail_b->d <= cur_a->d)
//        {
//            cur_a->next = tail_b;
//            cur_b->next = cur_a;
//
//            cur_a = tail_a;
//            tail_a = tail_a->next;
//
//            cur_b = cur_b->next;
//            //prtList(ch);
//        }
//
//        else if (tail_b->d > cur_a->d)
//        {
//            //prtList(ch);
//            cur_b = cur_b->next;
//            tail_b = tail_b->next;
//        }
//    }
//
//
//    return ch;
//}
//
//int main()
//{
//    node* p, * AH = NULL, * BH = NULL, * CH = NULL;
//
//    int K = 10, i;
//    for (i = 1; i <= K; i = i + 2)   //创建并初始化一个长度为K的链表，链表内容是自然数
//    {
//        p = new node;
//        p->d = i;
//
//        p->next = AH; //头插法（循环链表）
//        AH = p;
//    }
//
//
//    for (i = 2; i <= K + 5; i = i + 4)   //创建并初始化一个长度为K的链表，链表内容是自然数
//    {
//        p = new node;
//        p->d = i;
//
//        p->next = BH; //头插法
//        BH = p;
//    }
//    cout << "合并前A链表" << endl;
//    prtList(AH);  //显示链表1的当前值
//    cout << "合并前B链表" << endl;
//    prtList(BH);  //显示链表2的当前值
//
//    CH = combineList(AH, BH);
//
//    cout << "合并后" << endl;
//    prtList(CH);
//}


///*逆置线性表*/
//#include <iostream>
//
//using namespace std;
//
//typedef struct node  //定义节点数据结构
//{
//    int d;
//    struct node* next;
//};
//
//void prtList(node* head) //负责输出显示线性链表
//{
//    while (head != NULL)
//    {
//        cout << head->d << ' ';
//        head = head->next;
//    }
//    cout << endl;
//}
//
//node* invertList(node* head) //负责逆转链表的函数，您需要完成的作业
//{
//    //请同学们在这里写代码，完成以head为头指针的线性链表的逆序
//    node* pre = head;
//    node* cur = pre->next;
//    node* tail = cur->next;
//
//    head->next = NULL;
//
//    while (cur)
//    {
//        cur->next = pre;
//        pre = cur;
//        cur = tail;
//        if (tail != NULL)
//        {
//            tail = tail->next;
//        }
//    }
//    return pre;
//}
//
////node* invertList(node* head)
////{
////    node* pre = NULL;
////    node* cur = head;
////
////    while (cur != NULL)
////    {
////        node* nextNode = cur->next;
////        cur->next = pre;
////        pre = cur;
////        cur = nextNode;
////    }
////
////    return pre;
////}
//
//
//int main()
//{
//    node* p, * head = NULL, * newHead;
//
//    int K = 10, i;
//    for (i = 1; i < K; i++)   //创建并初始化一个长度为K的链表，链表内容是自然数
//    {
//        p = new node;
//        p->d = i;
//
//        p->next = head; //头插法
//        head = p;
//    }
//
//    prtList(head);  //显示链表的当前值
//
//    newHead = invertList(head);
//
//    prtList(newHead);  //显示链表的当前值
//}