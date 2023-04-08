#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(){
1
    string a = "123"; // a是个左值
    string &ref_a_left = a; // 左值引用指向左值
    string &&ref_a_right = std::move(a); // 通过std::move将左值转化为右值，可以被右值引用指向
    
    cout << a; // 打印结果：5
}