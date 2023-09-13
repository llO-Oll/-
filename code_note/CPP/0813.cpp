#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;
vector<string> result;
string path;
void backtracking(vector<string>& s, int startIndex,int size){
    if(path.size()==size){
        result.push_back(path);
        return;
    }
    for(int i=startIndex;i<s.size();i++){
        for(int j=0;j<s[i].size();j++)
        {
            path.push_back(s[i][j]);
            backtracking(s,i+1,size);
            path.pop_back();
        }
    }
}
int main() {
    string x,y,z;
    string a,b,c;
    cin>>x>>y>>z;
    vector<string> s;
    unordered_set<char> hash;
    for(auto ch:x){
        if(!hash.count(ch))  a.push_back(ch);
        hash.insert(ch);
    }
    for(auto ch:y){
        if(!hash.count(ch))  b.push_back(ch);
        hash.insert(ch);
    }
    for(auto ch:z){
        if(!hash.count(ch))  c.push_back(ch);
        hash.insert(ch);
    }
    s.push_back(a);
    s.push_back(b);
    s.push_back(c);
    //  每种变量取一个字母
    backtracking(s,0,1);
    cout<<result.size()<<endl;
    result.clear();
    path.clear();
    backtracking(s,0,2);
    cout<<result.size();
    return 0;
}