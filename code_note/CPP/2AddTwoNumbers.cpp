//遍历 vector 容器。
#include <iostream>
//需要引入 vector 头文件
#include <vector>
#include <map>
using namespace std;
vector<int> twoSum(vector<int>& nums, int target) {
    map<int,int> a;
    vector<int> b(2,-1);
    int i,j;
    for(i=0;i<nums.size();i++){
        if(a.count(target-nums[i])>0){
            b[1]=i;
            b[0]=a[target-nums[i]];
            break;
        }
        a[nums[i]]=i;
    }
    cout<<b[0]<<b[1]<<endl;
    return b;
}
int main()
{
    vector<int> a={2,7,9,11};
    twoSum(a,9);
    // system("pause");
    return 0;
}
