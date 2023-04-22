#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
unordered_map<int,int> getMap(int n){
    unordered_map<int, int> map;
    for(int i = 2; i <= n; i = i+2){
        map[i] = n-i+1;
    }
    return map;
}
int getNum(int a,int b){
    if(a > b){
        return getNum(b,a);
    }
    int sum = 0;
    unordered_map<int, int> x = getMap(a);
    unordered_map<int, int> y = getMap(b);

    for(unordered_map<int,int>::iterator it_x = x.begin(); it_x != x.end(); it_x++){
        for(unordered_map<int,int>::iterator it_y = x.begin(); it_y != x.end(); it_y++){
            sum += it_x->second * it_y->second;
        }
    }
    return sum;
}
int main() {
    cout<<getNum(3,3);
    return 0;
}

