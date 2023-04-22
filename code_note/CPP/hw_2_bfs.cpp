#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <set>
using namespace std;


int main(){
    /*  输入 构造二维图矩阵 */
    int n,m;
    cin>>n>>m;
    vector<set<int>> tree(n);   //  存储节点的数组，内部嵌套set 从小到大排序
    vector<int> isleave(n);     //  判断是否是叶子节点
    vector<int> father(n, 0);   //  存储父节点的数组形式的链表，例如father[1] = 0, father[2] = 1代表 0->1->2
    int a,b,k;
    for(int i=0;i<m;i++){
        cin>>a>>b;
        tree[a].insert(b);
        tree[b].insert(a);
        isleave[a]++;
        isleave[b]++;
    }
    cin >> k;
    vector<bool> block(k,false);
    for(int i = 0; i < k; i ++){
        int t;
        cin >> t;
        block[t] = true;    
    }
    /*  广度优先遍历 */
    queue<int> q;
    q.push(0);
    int res = -1;
    while(!q.empty()){
        int fnode = q.front();   //  父节点
        q.pop();
        if(isleave[fnode]== 1 && fnode > 0) {    //判断是否是叶子节点
            res = fnode;
            break;
        }
        for(set<int>::iterator it = tree[fnode].begin(); it != tree[fnode].end();it++){
            int node = *it;   //  当前子节点
            if(father[fnode] == node || block[node]) continue;
            father[node] = fnode;
            q.push(node);
        }
    }
    if(res == -1) {
        cout << "NULL" << endl;
        return 0;
    }
    vector<int> path;
    path.push_back(res);
    while(father[res] != 0) {
        path.push_back(father[res]);
        res = father[res];
    }
    cout << 0;
    for(int i = path.size() - 1; i >= 0; i--) {
        cout << "->" << path[i];
    }
    return 0;
}