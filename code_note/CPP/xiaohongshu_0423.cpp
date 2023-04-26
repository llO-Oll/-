#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;


vector <int> res;

void dfs(vector<vector<int>>& tree,vector<char>& color,vector <bool>& visited,int cur_point,int & count){
    for(int i=0;i<tree[cur_point].size();i++){
        int path = tree[cur_point][i];
        if(color[path]=='W' || visited[path]) continue;
        if(color[path] == 'R'){
            visited[path]=true;
            count++;
            dfs(tree,color,visited,path,count);
        }
    }
}
int main()
{
    int n,k;
    string s;
    cin >> n >> k;
    cin>>s;
    vector<vector<int>> tree(n+1);
    vector<char> color(n+1);
    vector <bool> visited(n+1,false);
    for(int i=1;i<=n;i++){
        color[i]=s[i-1];
    }
    for(int i = 0; i < n-1; i ++){
        int a, b;
        cin >> a >> b;
        tree[a].push_back(b);
        tree[b].push_back(a);
    }
    
    for(int i=1;i<tree.size();i++){
        int count =1;
        
        if(visited[i] || color[i]=='W') continue;
        visited[i]=true;
        dfs(tree,color,visited,i,count);
        res.push_back(count);
    }
    if(res.size()<k){
        cout << -1;
        return 0;
    }
    sort(res.begin(),res.end(),greater<int>());
    cout << res[k-1];
    return 0;
}