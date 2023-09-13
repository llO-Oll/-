#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;
vector<vector<int>> result;
vector<int> path;
void backtracking(vector<int>& nums, int startIndex){
    if(path.size()>0){
        result.push_back(path);
        for(int i=0;i<path.size();i++){
            cout<<path[i];
        }
        cout<<endl;
    }
    if(startIndex>=nums.size()){
        return;
    }
    path.push_back(nums[startIndex]);
    backtracking(nums,startIndex+1);
    path.pop_back(); 
}
int main() {
    int n;
    cin>>n;
    vector<int> a,b;
    for(int i=0;i<n;i++){
        int temp;
        cin>>temp;
        a.push_back(temp);
    }
    for(int i=0;i<n;i++){
        int temp;
        cin>>temp;
        b.push_back(temp);
    }
    for(int i=0;i<n;i++){
        backtracking(a,i);
    }
    for(int i=0;i<n;i++){
        backtracking(a,i);
    }
    
    cout<<result.size();
    return 0;
}
// 64 位输出请用 printf("%lld")