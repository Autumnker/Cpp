//#define _CRT_SECURE_NO_WARNINGS 1
//
//#include<iostream>
//using namespace std;
////Box类的定义及成员函数书写
////***begin***
//class  Box
//{
//private:
//    float length;
//    float width;
//    float heigh;
//
//public:
//    Box()
//    {
//        length = 0.0; width = 0.0; heigh = 0.0;
//    }
//    Box(float len = 0, float wid = 0, float hei = 0)
//    {
//        length = len; width = wid; heigh = hei;
//    }
//
//    float getVolume()
//    {
//        return length * width * heigh;
//    }
//};
////***end***
//int main(int argc, char* argv[])
//{
//    Box b1(4);
//    Box b2(4, 5);
//    Box b3(4, 10, 8);
//    cout << b1.getVolume() << "," << b2.getVolume() << "," << b3.getVolume();
//}


///*对象数组的使用*/
//#include<iostream>
//using namespace std;
////Student类及相关函数的书写
////***begin***
//class Student {
//
//public:
//    string No;
//    string Name;
//    float Score;
//
//public:
//
//    Student()
//    {
//        No = "\0";
//        Name = "\0";
//        Score = 0.0;
//    }
//
//    void setNo(string no)
//    {
//        No = no;
//    }
//    void setName(string name)
//    {
//        Name = name;
//    }
//    void setScore(float score)
//    {
//        Score = score;
//    }
//
//    void display()
//    {
//        cout << No << '/' << Name << '/' << Score;
//    }
//};
//
//Student max(Student* stu)
//{
//    Student s;
//    int score = 0;
//    int i = 0;
//    for (i = 0; i < 5; i++)
//    {
//        if (stu[i].Score > score)
//        {
//            score = stu[i].Score;
//            s = stu[i];
//        }
//    }
//
//    return s;
//}
//
//
////***end***
//int main(int argc, char* argv[])
//{
//    Student stu[5];
//    string name, no;
//    float score;
//    for (int i = 0; i < 5; i++)
//    {
//        cin >> no >> name >> score;
//        stu[i].setNo(no);
//        stu[i].setName(name);
//        stu[i].setScore(score);
//    }
//    Student temp = max(stu);
//    temp.display();
//}


///*类的静态成员的使用*/
//#include <iostream>
//using namespace std;
////class Salesman ，请在这里完成Saleman类
//class Salesman
//{
//public:
//   int num;
//   int quantity;
//   float price;
//   //总销量
//   static int salesamount;
//   //总售价
//   static float total_price;
//
//    void CalculateQuantity()
//    {
//        Salesman::salesamount += quantity;
//    }
//
//    void CalculateSale()
//    {
//        Salesman::total_price += price * quantity;
//    }
//    //平均价格
//    static float CalculateAveragePrice()
//    {
//        return total_price / salesamount;
//    }
//};
////
//
//int Salesman::salesamount = 0;
//float Salesman::total_price = 0;
//
//int main()
//{
//
//    Salesman  sa[3];
//    for (int i = 0; i < 3; i++)
//    {
//        cin >> sa[i].num >> sa[i].quantity >> sa[i].price;
//        sa[i].CalculateQuantity();
//        sa[i].CalculateSale();
//    }
//    cout << "sum=" << Salesman::total_price << endl;
//    cout << "averageprice=" << Salesman::CalculateAveragePrice();
//}

///*友元类*/
//#include <iostream>
//using namespace std;
////请在下面完成Time类和Date类的编写
//class Time {
//private:
//
//    int sec;
//    int min;
//    int hour;
//    int day;
//    int month;
//    int year;
//
//public:
//    Time(int s, int mi, int h, int d, int mo, int y)
//    {
//        sec = s; min = mi; hour = h; day = d;
//        month = mo; year = y;
//    }
//
//    friend class Date;
//
//};
//
//class Date {
//public:
//
//    int day;
//    int month;
//    int year;
//
//    Date()
//    {
//        day = 0; month = 0; year = 0;
//    }
//
//    void display(Time& t)
//    {
//        cout << t.hour << ":" << t.min << ":" << t.sec;
//    }
//};
//
//
//int main()
//{
//    int sec, min, hour, day, mon, year;
//    cin >> sec >> min >> hour >> day >> mon >> year;
//    Time t1(sec, min, hour, day, mon, year);
//    Date d;
//    d.display(t1);
//
//    return 0;
//}