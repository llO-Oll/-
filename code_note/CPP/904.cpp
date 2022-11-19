#include<iostream>
#include<vector>


using namespace std;

class Solution{
public:
    int totalFruit(vector<int>& fruits) {
    /*
    Args:
        l、r        :左右指针
        cls1、cls2  :当前区间的两个数字
        flag        :=true当前区间存在两个不同数字;=false当前区间存在一个数字
        r_start     :记录第二个数字的起始位置
    */
        int r=0;
        int l=0;
        int cls1 = fruits[l];
        int cls2 = fruits[r];
        int r_start = 0;
        int max_length = 0;
        for(r = 0;r < fruits.size(); r++){
            //如果此时区间中有新数字;
            if(fruits[r] != cls1 && fruits[r] != cls2){
                max_length = max(max_length ,r-l); 
                l = r_start; 
                cls1 = fruits[l];
                cls2 = fruits[r]; 
                r_start = r;
                cout<<"    "<<"l:"<<l<<" r:"<<r<<" length:"<<max_length<<" cls1:"<<cls1<<" cls2:"<<cls2<<endl;
            }
            // 如果此时区间中没有新数字
            if((fruits[r] == cls1 || fruits[r] == cls2) && r>0){
                if(fruits[r]!=fruits[r-1]){
                    r_start = r;
                }
                max_length = max(max_length,r-l+1);
                cout<<"l:"<<l<<" r:"<<r<<" length:"<<max_length<<" cls1:"<<cls1<<" cls2:"<<cls2<<endl;
            }
        }
        return max_length;
    }
};

int main(){
    Solution s;
    vector<int> a = {0,1,6,6,4,4,6};
    cout<<s.totalFruit(a)<<endl;
    vector<int> b = {3,3,3,1,2,1,1,2,3,3,4};
    cout<<s.totalFruit(b)<<endl;
    return 0;
}