#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class Solution{
    public:
    int coinChange1(vector<int>& coins,int amount){
        int m=coins.size();
        vector<int> dp(amount+1,amount+2);//dp[i]表示金额为i的最少硬币数
        dp[0]=0;
        for(int i=1;i<=amount;i++){
            for(auto &c:coins){
                if(i>c)
                    dp[i]=min(dp[i],dp[i-c]+1);
            }
        }
        return dp[amount]==amount+2?-1:dp[amount];
    }

    int coinChange2(vector<int>& coins,int amount){
        sort(coins.begin(),coins.end(),greater<int>());
        for(auto &c:coins){
            cout<<c;
        }
        int m=coins.size();
        int res=amount+2;
        for(int i=0;i<m;i++){
            dfs(i,0,res,coins,amount);
        }
        return res==amount+2?-1:res;

    }
    void dfs(int i,int num,int &res,vector<int>& coins,int amount){
        if(amount==0){
            res=min(res,num);
            return;
        }
        for(int j=i;j<coins.size();j++){
            if((res-num)*coins[j]<amount)
                break;
            if(coins[j]>amount)
                continue;
            dfs(j,num+1,res,coins,amount-coins[j]);
        }
    }
};

int main(){
    Solution sl;
    vector<int> coins{1, 2, 5};
    int amount=11;
    int a=sl.coinChange2(coins,amount);
    cout<<a<<endl;
};