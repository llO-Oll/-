#include<iostream>
#include<vector>
using namespace std;

int main(){
    string s;
    cin>>s;

    /* 存储m,n */
    int n=s[0],m=s[2];

    /* new一个数组存矩阵 默认全白*/
    vector<vector<char>> grid(n,vector<char>(m,'W'));
    int i=0;
    while( cin >> s ){
        for(int j=0;j<s.size();j++){
            if(s[j]=='R')
            grid[i][j]='R';
        }
        i++;
    }
    for(int i=0; i<n;++i){
        for(int j=0;j<m;++j){
            // 以下为核心代码

            char temp = grid[i][j];
            grid[i][j] == 'W';
            int num_redislands = 0;
            for (int r = 0; r < n; ++r) {
                for (int c = 0; c < m; ++c) {
                    if (grid[r][c] == 'R') {
                        ++num_redislands;
                        dfs(grid, r, c);
                    }
                }
            }
            // 还原矩阵
            if(temp=='R')
                grid[i][j] == 'R';
            
            // 输出数目
            cout<<num_redislands;
        }
        // 输出回车
        cout<<"\n";
    }
}

// 深度优先遍历
void dfs(vector<vector<char>>& grid, int r, int c) {
    int nr = grid.size();
    int nc = grid[0].size();
    char temp = grid[r][c];
    grid[r][c] = 'W';
    if (r - 1 >= 0 && grid[r-1][c] == 'R') dfs(grid, r - 1, c);
    if (r + 1 < nr && grid[r+1][c] == 'R') dfs(grid, r + 1, c);
    if (c - 1 >= 0 && grid[r][c-1] == 'R') dfs(grid, r, c - 1);
    if (c + 1 < nc && grid[r][c+1] == 'R') dfs(grid, r, c + 1);
    grid[r][c] = temp;
}