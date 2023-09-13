#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

int main(){
    int n,m;
    cin>>n>>m;
    /*
        顾客中若有重复想要的商品，则商店从有购买重复商品的顾客之后就开始卖不出商品。
    */
    string s,c;
    cin>>s;
    cin>>c;
    string c1;
    unordered_set<char> hash_c;
    unordered_set<char> hash_s;
    for(int i=0;i<s.size();i++){
        hash_s.insert(s[i]);
    }
    for(int i=0;i<c.size();i++){
        if(hash_c.count(c[i])){
            break;
        }
        hash_c.insert(c[i]);
        c1.push_back(c[i]);
    }
    int ans=0;
    for(int i=0;i<c1.size();i++){
        if(hash_s.count(c1[i])){
            ans++;
        }
    }
    cout<<ans;

}