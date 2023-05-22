#include <iostream>
#include <string>
using namespace std;
class A{
	static int a;
};
int A::a = 1;
int main(){
	int a =123;
	int *p;
	p = &a;
	cout << *p;
}