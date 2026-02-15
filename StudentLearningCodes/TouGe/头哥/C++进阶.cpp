#define _CRT_SECURE_NO_WARNINGS 1

///*C++进阶*/
//#include<iostream>
//using namespace std;
//
//float sum(float a = 0.0, float b = 10.0, float c = 10.0)
//{
//	return a + b + c;
//}
//
//int main(int argc, char* argv[])
//{
//	float a, b, c;
//	cin >> a >> b >> c;
//	cout << sum(a) << ' ' << sum(a, b) << ' ' << sum(a, b, c) << endl;
//	return 0;
//}


///*数字交换*/
//#include<iostream>
//using namespace std;
//
//void swap(int& a, int& b)
//{
//	int temp = a;
//	a = b;
//	b = temp;
//	return;
//}
//
//int main(int argc, char* argv[])
//{
//	int a, b;
//	cin >> a >> b;
//	int& c = a, & d = b;
//	swap(c, d);
//	cout << a << ',' << b << endl;
//	return 0;
//}

///*模板函数,泛型编程*/
//#include<iostream>
//#include<string>
//
//using namespace std;
//
//template <typename T>
//void sort(T p[], int n)
//{
//    int i = 0, j = 0;
//    for (i = 0; i < n; i++)
//    {
//        for (j = 0; j < n - 1; j++)
//        {
//            if (p[j] > p[j + 1])
//            {
//                T temp = p[j];
//                p[j] = p[j + 1];
//                p[j + 1] = temp;
//            }
//        }
//    }
//
//    for (i = 0; i < n; i++)
//    {
//        cout << p[i];
//        if (i != n - 1)
//        {
//            cout << ',';
//        }
//    }
//    cout << endl;
//}
//
//int main(int argc, char* argv[])
//{
//    int n = 0;
//    cin >> n;
//    int* x = new int[n];
//    float* y = new float[n];
//
//    for (int i = 0; i < n; i++)
//        cin >> x[i];
//    for (int i = 0; i < n; i++)
//        cin >> y[i];
//    sort(x, n);
//    sort(y, n);
//
//    delete(x);
//    delete(y);
//
//    return 0;
//}



///*车的类*/
//#include <iostream>
//using namespace std;
//
///********* Begin *********/
////在此处实现一个汽车类
//class Car
//{
//public:
//    string door;
//    string light;
//    int speed;
//
//    //构造函数
//    Car()
//    {
//        door = "OFF";
//        light = "OFF";
//        speed = 0;
//    }
//
//
//    //车门 1 开 2 关//车灯 3 开 4 关//速度 5 加速 6 减速
//    void change(int d);
//    void prin()
//    {
//        cout << "车门 " << door << endl;
//        cout << "车灯 " << light << endl;
//        cout << "速度 " << speed << endl;
//    }
//};
///********* End *********/
//
//void Car::change(int d)
//{
//    if (d == 1)
//    {
//        door = "ON";
//    }
//    else if (d == 2)
//    {
//        door = "OFF";
//    }
//
//    if (d == 3)
//    {
//        light = "ON";
//    }
//    else if (d == 4)
//    {
//        light = "OFF";
//    }
//
//    if (d == 6 && speed >= 10)
//    {
//        speed -= 10;
//    }
//    else if (d == 5)
//    {
//        speed += 10;
//    }
//
//}
//
//
//int main()
//{
//    /********* Begin *********/
//    //在此处解析执行输出汽车的整体状态
//    char cmds[25];
//    for (int i = 0; i < 25; i++)
//    {
//        cmds[i] = '\0';
//    }
//    cin >> cmds;
//
//
//    Car car;
//
//    int i = 0;
//    while (cmds[i] != '\0')
//    {
//        int ab = (int)(cmds[i] - '0');
//
//        car.change(ab);
//        i ++;
//    }
//
//    car.prin();
//
//    /********* End *********/
//}


///*稀疏向量*/
//#include <cstdlib>
//#include<iostream>
//using namespace std;
//
////Vec为单链表结点数据类型
//struct Vec {
//    int val;
//    int pos;
//    struct Vec* next;
//};
//
//Vec* Create();
//void Init(Vec* V, int d);//d是要创建的节点个数
//void Insert(Vec* V, int _val, int _pos);
//void Destroy(Vec* V);
//int Multiply(Vec* V1, Vec* V2);
//
//void show(Vec* V)
//{
//    Vec* p = V;
//    while (p->next)
//    {
//        p = p->next;
//        cout << "V->pos: " << p->pos << endl;
//        cout << "V->val: " << p->val << endl;
//        cout << endl;
//    }
//}
//
//int main() {//补齐代码
//    Vec* V1 = Create();
//    Vec* V2 = Create();
//    int d1 = 0;
//    scanf("%d ", &d1);
//    Init(V1,d1);
//    //show(V1);
//    int d2 = 0;
//    scanf("%d ", &d2);
//    Init(V2, d2);
//    //show(V2);
//    printf("%d\n", Multiply(V1, V2));
//    //Destroy(V1);
//    //Destroy(V2);
//
//
//
//    return 0;
//}
//
//Vec* Create() {
//    Vec* V = new Vec; //头结点
//    V->next = NULL;
//    V->pos = -1;
//    V->val = 0;
//
//    return V;
//}
//void Init(Vec* V, int d) {
//    if (d != 0)
//    {
//        int val = 0;
//        Vec* now = V;
//        for (int i = 0; i < d; i++) {
//            scanf("%d ", &val);
//            if (val != 0) { //将非零元素放入到新创建的结点中，然后插入到单链表中
//                //创建新节点
//                Vec* p = new Vec;
//                //节点赋值
//                p->pos = i;
//                p->val = val;
//                //节点尾插法
//                //printf("\n%p\n", now);
//                p->next = now->next;
//                now->next = p;
//                //指针后移
//                now = now->next;
//            }
//            else
//            {
//                i = i - 1;
//                d = d - 1;
//            }
//        }
//    }
//    else
//    {
//        return;
//    }
//}
//void Insert(Vec* V, int p, int q) { //将结点q插入到单链表V的p结点后
//    ////当p 、q 是节点时
//    //if (q->val != 0)
//    //{
//    //    //获取节点的编号
//    //    int num = p->pos + 1;
//    //    //在V中定位该编号的地址
//    //    int i = 0;
//    //    Vec* now = V;
//    //    for (i = 0; i < num; i++)
//    //    {
//    //        now = now->next;
//    //    }
//    //    //将q插入
//    //    q->next = now->next;
//    //    now->next = q;
//    //}
//    //else
//    //{
//    //    return;
//    //}
//
//
//
//    //当 p 、q 是数字时
//    if (q != 0)
//    {
//        //获取节点的编号
//        int num = p + 1;
//        //在V中定位该编号的地址
//        int i = 0;
//        Vec* now = V;
//        for (i = 0; i < num; i++)
//        {
//            now = now->next;
//        }
//
//        //创建一个数值为 q 的节点
//        Vec* q_in = new Vec;
//        q_in->val = q;
//        //将q插入
//        q_in->next = now->next;
//        now->next = q_in;
//    }
//    else
//    {
//        return;
//    }
//}
//void Destroy(Vec* V) { //摧毁单链表V
//    Vec* now = V->next;
//    Vec* pre = V;
//    free(pre);
//    while (pre != NULL) {//请补齐
//        pre = now;
//        if (now->next != NULL)
//        {
//            now = now->next;
//        }
//        free(pre);
//    }
//}
//
//int Multiply(Vec* V1, Vec* V2) {//请补齐
//    int total = 0;
//    Vec* p1 = V1->next, * p2 = V2->next;
//    
//    while (p1 != NULL && p2 != NULL)
//    {
//        total += (p1->val) * (p2->val);
//        p1 = p1->next;
//        p2 = p2->next;
//    }
//
//    return total;
//}