/*===============================================
*   文件名称：test.cpp
*   创 建 者：Autumnker
*   创建日期：2025年08月07日
*   描    述：
================================================*/
#include <iostream>

using namespace std;
/*基础门*/
class BaseDoor {
public:
    BaseDoor() { base_val = -1; };
    BaseDoor(int val) :base_val(val) {};
    virtual void open() {
        cout << "基础门打开,花费" << base_val << "点力量" << endl;
    }
    virtual void close() {
        cout << "基础门关闭,花费" << base_val << "点力量" << endl;
    }
private:
    int base_val;
};
/*木门*/
class WoodenDoor :public BaseDoor {
public:
    WoodenDoor() { wooden_val = -1; };
    WoodenDoor(int val, int base_val) :wooden_val(val), BaseDoor(base_val) {};
    void open() override {
        cout << "木门打开,花费" << wooden_val << "点力量" << endl;
    }
private:
    int wooden_val;
};
/*钢门*/
class SteelDoor :public BaseDoor {
public:
    SteelDoor() { steel_val = -1; };
    SteelDoor(int val, int base_val) :steel_val(val), BaseDoor(base_val) {};
    void open() override {
        cout << "钢门打开,花费" << steel_val << "点力量" << endl;
    }
private:
    int steel_val;
};

/*测试*/
void test_open(BaseDoor* door) {
    door->open();
}

void (*fun)();

int main(int argc, char* argv[])
{
    BaseDoor* door = new BaseDoor(1);
    test_open(door);

    /*直接访问虚函数表*/
    long* p = (long*)door;      // 将对象指针转为只能解引用出前八个字节内容的指针
    long* v = (long*)(*p);      // 将对象数据的前八个字节取出（这八个字节为虚函数表指针）
    fun = (void(*)(void))(*v);  // 将虚函数表指针的首地址强转为函数指针
    cout << "基类虚函数1地址:" << (void*)(*v) << endl;
    fun();
    fun = (void(*)(void))(*(++v));
    cout << "基类虚函数2地址:" << (void*)(*v) << endl;
    fun();
    cout << "------------------------------------------" << endl;

    door = new WoodenDoor(1, 2);
    test_open(door);
    long* tp = (long*)door;
    long* fp = (long*)(*tp);
    fun = (void(*)(void))(*fp);
    cout << "木门虚函数1地址:" << (void*)(*fp) << endl;
    cout << "木门虚函数2地址:" << (void*)(*(++fp)) << endl;
    fun();
    cout << "------------------------------------------" << endl;

    door = new SteelDoor(3, 4);
    test_open(door);
    long* tp2 = (long*)door;
    long* fp2 = (long*)(*tp2);
    fun = (void(*)(void))(*fp2);
    cout << "钢门虚函数1地址:" << (void*)(*fp2) << endl;
    cout << "钢门虚函数2地址:" << (void*)(*(++fp2)) << endl;
    fun();
    cout << "------------------------------------------" << endl;

    return 0;
}
