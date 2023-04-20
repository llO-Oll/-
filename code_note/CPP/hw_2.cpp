#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;
int n, m, k, layer, minx = 10010;
vector<int> tr[10010];
bool st[10010], block[10010];
vector <int> pa, ans;
void dfs(int x, int layer){
    pa.push_back(x);
    for(int i = 0; i < tr[x].size(); i ++){
        int idx = tr[x][i];
        if(st[idx] || block[idx]) continue;
        st[x] = true;
        dfs(idx, layer + 1);
        st[x] = false;
    }
    if(tr[x].size() == 1 && layer < minx){
        minx = layer;
        ans = pa;
    }
    pa.pop_back();
}
int main()
{
    cin >> n >> m;
    for(int i = 0; i < m; i ++){
        int a, b;
        cin >> a >> b;
        tr[a].push_back(b);
        tr[b].push_back(a);
    }
    cin >> k;
    for(int i = 0; i < k; i ++){
        int t;
        cin >> t;
        block[t] = true;    
    }
    dfs(0, 1);
    if(ans.size()==1){
        cout<<"NULL";
        return 0;
    }  
    for(int i = 0; i < ans.size(); i ++){
        if(i != 0) cout << "->";
        cout << ans[i];
    }
    return 0;
}