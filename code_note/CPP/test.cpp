#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <string>

using namespace std;
int main(){
    int i;
    int maxlen = -1;
    string s;
    cin>>s;
    vector<int> index (26, -1);
    for(int i=0;i<s.size();i++){
        int pos = s[i] - 'a';
        if(index[pos] == -1){
            index[pos]=i;
        }else{
            maxlen = max(maxlen, i - index[pos] -1);
        }
    }
    cout << maxlen;
    return 0;
}