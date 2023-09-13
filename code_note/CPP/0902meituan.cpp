#include<iostream>
#include<vector>
using namespace std;

int main(){
    int n;
    cin>>n;
    vector<int> v;
    for(int i=0;i<n;i++){
        int a;
        cin >> a;
        v.push_back(a);
    }
    
    vector<int> result;
    result.push_back(v[0]+1);
    int j=0;
    for(int i=0;i<n;i++){
        if(result[j]>v[i]){
            result.push_back(v[i]);
            j++;
        }else{
            result.push_back(v[i]+1);
            result.push_back(v[i]);
            j+=2;
        }
    }
    cout<<result.size()<<endl;
    for(auto r:result){
        cout<<r<<" ";
    }
    return 0;
};