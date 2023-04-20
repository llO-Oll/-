#include <iostream>
#include <string>
using namespace std;
class ADT{
	void sss(){

	}
	int i;
	static int j;
public:
	ADT(){
		this->i = 10;
		
		cout << "ADT construct i=" << this->i << "j="<<j <<endl;
	}
	~ADT(){
		cout << "ADT destruct" << endl;
	}
	static void f(){
		cout<<"sss"<<endl;
	};
};
// int ADT::j = 0;
int main()
{
	char *p = new(nothrow) char[sizeof(ADT)+1];
	if (p == NULL) {
		cout << "alloc failed" << endl;
	}
	ADT *q = new(p) ADT;  //placement new:不必担心失败，只要p所指对象的的空间足够ADT创建即可
	//delete q;//错误!不能在此处调用delete q;
	q->ADT::~ADT();//显示调用析构函数
	// q->~ADT();
	delete[] p;
	return 0;
}

//输出结果：
//ADT construct i=10j=100
//ADT destruct
