//遍历 vector 容器。
#include <iostream>
//需要引入 vector 头文件
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
using namespace std;
class Solution{
public:

int findMaxForm(vector<string>& strs, int m, int n){
    int s=strs.size();
    vector<vector<vector<int>>> dp(s+1,vector<vector<int>>(m+1,vector<int>(n+1,0)));
    for(int k=1;k<=s;k++){
        auto p=count(strs[k-1]);
        for(int i=0;i<=m;i++){
            for(int j=0;j<=n;j++){
                if(i>=p.first && j>=p.second){
                    dp[k][i][j]=max(dp[k-1][i][j],dp[k-1][i-p.first][j-p.second]+1);
                }else{
                    dp[k][i][j]=dp[k-1][i][j];
                }
            }
        }             
    }  
    return dp[s][m][n];
}
pair<int,int> count(const string &str){
    int count0=str.length(),count1=0;
    for(const char &c:str){
        if(c == '1'){
            ++count1;
            --count0;
        }
    }
    return make_pair(count0,count1);
}

};


int main()
{
    Solution sl; 
    vector<string> strs={"10", "0001", "111001", "1", "0"};
    int m=5,n=3;
    int res=sl.findMaxForm(strs,m,n);
    cout<<res<<endl;
    system("pause");
    return 0;
}