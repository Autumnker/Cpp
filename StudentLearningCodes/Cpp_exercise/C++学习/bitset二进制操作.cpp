//#define _CRT_SECURE_NO_WARNINGS 1
//
///*bitset位运算（是存储二进制数的字符数组，a[0]对应二进制数的最低位）
//1.创建存储二进制的字符数组并初始化为 0 
//2.用 unsigned int 型数据初始化字符数组
//3.用二进制字符串初始化字符数组，字符串拼接在二进制数的最低位
//4.这些全是关于 1 的操作 b.any() ; b.none() ; b.count() ; b.test(i) ; b.set(i) ;
//5.这些全是关于 0 的操作 b.reset() ; b.reset(i) ;
//6.这些全是取反操作 b.flip() ; b.flip(i) ;
//7.unsigned long a = b.to_ulong(); 将二进制转换为 unsigned long 类型 ; b.size() 获取二进制数的位数
//*/
//
//#include<bitset>
//#include<iostream>
//using namespace std;
//
//int main(void)
//{
//	//【1】将 5 个二进制位初始化为 0 
//	bitset<5> b0;
//	cout << "initBitset:" << b0 << endl;
//
//	//【2】用 unsinged int 型的数据初始化二进制数组
//	unsigned int a = 10;
//	bitset<5> b1(a);
//	cout << "unsigned int bitset:" << b1 << endl;
//
//	//【3】用字符串初始化二进制数组
//	string str = "10110";
//	bitset<10> b2(str);
//	cout << "str for sitset;" << b2 << endl;
//
//	cout << "\n指定初始化二进制数组的字符串的长度，字符串为:11001010\n" << endl;
//	string str_half = "11001010";
//	bitset<5>bit_1(str_half, 0, 5);//从str的 0 号位置开始，取五个字符用于初始化 bit_1
//	cout << "after bit_1(str_half,0,5):" << bit_1 << endl;
//	
//	//【4】关于 1 的操作
//	cout << "\ncreat a bitset with only three 1s: 10101\n" << endl;
//	string str2 = "10101";
//	bitset<5>b3(str2);
//	cout << "b3.any() means that there at least one 1 in b3:" << b3.any() << endl;
//	cout << "b3.none() means there is no 1 in b3:" << b3.none() << endl;
//	cout << "b3.count() means how many 1s in b3:" << b3.count() << endl;
//	cout << "b3.test(i),i = 2 means if there is 1 when i = 2:" << b3.test(2) << endl;
//	b3.set(1);//这里的 1 是从二进制低位到高位的序号
//	cout << "b3.set(i),i = 1 means turn it to 1 when i = 1:" << b3 << endl;
//	
//	//【5】关于 0 的操作
//	cout << "\ncreat a bitset with two 1s: 01001\n" << endl;
//	bitset<5>b4("01001");
//	cout << "b4:" << b4 << endl;
//	b4.reset(0);
//	cout << "after b4.reset(0):" << b4 << endl;
//	b4.reset();
//	cout << "b4 was .reset():" << b4 << endl;
//
//	//【6】取反操作
//	cout << "\ncreat a bitset: 10101011\n" << endl;
//	bitset<8>b5("10101011");
//	cout << "b5:" << b5 << endl;
//	b5.flip();
//	cout << "after b5.flip():" << b5 << endl;
//	b5.flip(0);
//	b5.flip(1);
//	cout << "after b5.flip(0),b5.flip(1):" << b5 << endl;
//
//	//【7】数据类型转换和获取长度的操作
//	cout << "\ncreat a bitset:11011101\n" << endl;
//	bitset<8>b6("11011101");
//	cout << "b6:" << b6 << endl;
//	unsigned long ulong_num = b6.to_ulong();
//	string string_num = b6.to_string();
//	cout << "after b6.to_ulong()这个是将二进制转换为无符号十进制:" << ulong_num << endl;
//	cout << "after b6.to_string():" << string_num << endl;
//
//	int xy = -4;//像循环链表一样，最小前面是最大。
//	bitset<5> b7(xy);
//	cout << "\n用 -4 对其进行初始化，发现它相当于循环链表\nb7:" << b7 << endl;
//
//	return 0;
//}