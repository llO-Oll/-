#include<iostream>
#include<vector>

class UF{
std::vector<int> fa;
std::vector<int> rank;
public:
    void init(int n){
        for(int i=0;i<n;i++){
            fa.push_back(i);
            rank.push_back(1);
        }
    }
    int find(int x){
        return fa[x] = fa[x]==x?x:find(fa[x]);
    }

    void merge(int i,int j){
        int x = find(i),y=find(j);
        if(rank[x]<=rank[y]){
            fa[y] = find(x);
        }else{
            fa[x] = find(y);
        }
        //深度一样，rank[y]++
        if(rank[x]==rank[y]){
            rank[y]++;
        }
    }

    bool isConnected(int i, int j){
        if(find(i) == find(j)){
            return true;
        }else{
            return false;
        }
    }
};

class Solution {
public:
    std::vector<int> findRedundantConnection(std::vector<std::vector<int>>& edges) {
        int n=edges.size();
        UF uf;
        uf.init(n);
        for(auto &  e:edges){
            int x = e[0],y=e[1];
            if(uf.isConnected(x,y)){
                return e;
            }
            uf.merge(x,y);
        }
        return std::vector<int>{-1,-1};
    }
};


int main(){
    std::vector<std::vector<int>> edges{{1,2}, {2,3},{3,4},{1,4},{1,5}};
    Solution s;
    std::vector<int> res(s.findRedundantConnection(edges));
    std::cout<< res[0]<<res[1];
}