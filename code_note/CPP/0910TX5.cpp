#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

int findLength(string s1, string s2) {
    vector<vector<int>> dp (s1.size() + 1, vector<int>(s2.size() + 1, 0));
    int result = 0;
    for (int i = 1; i <= s1.size(); i++) {
        for (int j = 1; j <= s2.size(); j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            if (dp[i][j] > result) result = dp[i][j];
        }
    }
    return result;
}

int N = 501; // n根据题目中节点数量而定，一般比节点数量大一点就好
vector<int> father = vector<int> (N, 0); // C++里的一种数组结构

// 并查集初始化
void init() {
    for (int i = 0; i < N; ++i) {
        father[i] = i;
    }
}
// 并查集里寻根的过程
int find(int u) {
    return u == father[u] ? u : father[u] = find(father[u]); // 路径压缩
}

// 判断 u 和 v是否找到同一个根
bool isSame(int u, int v) {
    u = find(u);
    v = find(v);
    return u == v;
}

// 将v->u 这条边加入并查集
void join(int u, int v) {
    u = find(u); // 寻找u的根
    v = find(v); // 寻找v的根
    if (u == v) return ; // 如果发现根相同，则说明在一个集合，不用两个节点相连直接返回
    father[v] = u;
}

int main(){
    int n,k;
    cin>>n>>k;
    vector<string> S(n);
    for(int i=0;i<n;i++){
        cin>>S[i];
    }
    vector<int> dels(n);
    for(int i=0;i<n;i++){
        cin>>dels[i];
    }
    init();
    for(int i=0;i<S.size()-1;i++){
        for(int j=i+1;j<S.size();j++){
            int len = findLength(S[i],S[j]);
            if(len>=k){
                join(i,j);
            }
        }
    }
    int res=0;
    for(int i=0;i<n;i++){
        if(father[i]==i){
            res++;
        }
    }
    cout<<res<<endl;
    unordered_set<int> hash;
    for(int x=0;x<n;x++){
        init();
        hash.insert(dels[x]-1);
        for(int i=0;i<S.size()-1;i++){
            if(hash.count(i))   continue;
            for(int j=i+1;j<S.size();j++){
                if(hash.count(j))   continue;
                int len = findLength(S[i],S[j]);
                if(len>=k){
                    join(i,j);
                }
            }
        }
        res=0;
        for(int i=0;i<n;i++){
            if(father[i]==i){
                res++;
            }
        }
        cout<<res-x-1<<endl;
    } 
}