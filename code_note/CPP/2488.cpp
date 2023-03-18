#include<iostream>
#include<vector>

using namespace std;

class Solution {
public:
    int countSubarrays(vector<int>& nums, int k) {
        int kindex;
        for(int i=0;i<nums.size();i++){
            if(nums[i]<k){
                nums[i]=-1;
            }else if(nums[i]==k){
                nums[i]=0;
                kindex=i;
            }else{
                nums[i]=1;
            }
        }
        int res=1;
        int l,r=0;
        for(int l=0;l<nums.size();l++){
            int r=l;
            int sum=0;
            while(r<nums.size()){
                sum+=nums[r];
                if(sum==0&&l<=kindex&&r>=kindex){
                    res++;
                }
                r++;
            }
        }
        return res;
    }
};

int main(){
    Solution s;
    vector<int> nums{3,2,1,4,5};
    int k = 4;
    cout<<s.countSubarrays(nums,k)<<endl;
}