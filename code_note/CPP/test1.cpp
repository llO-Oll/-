#include<iostream>
#include<algorithm>
#include<vector>
#include<functional>
using namespace std;

class GreaterFive{
public:
    bool operator()(int val){
        return val > 5;
    }
    static int func(){

    };
};

void test01(){
    vector<int> v;
    for(int i=0;i<10;i++){
        v.push_back(i);
    }
    vector<int>::iterator it = find_if(v.begin(),v.end(),GreaterFive());
    if(it==v.end()){
        cout<<"没找�?"<<endl;
    }else{
        cout<<"找到大于5的数�?"<< *it <<endl;
    }
}
int main(){
    test01();
}