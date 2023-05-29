#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

const int maxn = 20005;

int n,m;
vector<int> e[maxn];
int degree[maxn];

vector<int> topo_sort(int d[], int n) {
    vector<int> res;
    queue<int> q;
	for(int i=1;i<=n;i++) {
		if(d[i]==0) {
            q.push(i);
        }
	}
    while(!q.empty()) {
        int u = q.front(); q.pop();
        res.push_back(u);
        for(int i=0;i<int(e[u].size());i++) {
            int v = e[u][i];
            d[v]--;
            if(d[v]==0) {
                q.push(v);
            }
        }
    }
    return res;
}

// s-起点，t-终点，n-顶点数
int count_path_number(int d[],int s,int t,int n) {
    vector<int> a = topo_sort(d,n);
    int dp[n+5];
    memset(dp,0,sizeof(dp));

    for(auto i:a) cout << i << ' ';
    dp[t] = 1; //从终点向起点反向遍历
    for(int i=n;i>=1;i--) {
        int u = a[i-1];
        for(int j=0;j<int(e[u].size());j++) {
            int v = e[u][j];
            dp[u] += dp[v];
        }
    }
    return dp[s];
}

int main() {
    vector<vector<int>> e;
    
    cout << count_path_number(degree,1,n,n);
    return 0;
}

/* 样例
5 7
1 2
2 3
1 3
3 5
2 5
4 1
4 5
*/
