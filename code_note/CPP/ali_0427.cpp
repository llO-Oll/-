#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n,m,k;
    cin >> n>>m>>k;
    if( k<min(n,m) || ((k % n) && (k % m)) ){
        cout<<-1;
        return 0;
    }
    vector<vector<char>> res(n,vector<char>(m));
    if( k>=n && !(k % n) ){
        int nums_2 = k/n;
        for(int i=0;i<n;i++){
            for(int j=0;j<nums_2;j++){
                if(i%2){
                    res[i][2*j] = (j%2)?'0':'1';
                    res[i][2*j+1] = (j%2)?'0':'1';
                }else{
                    res[i][2*j] = (j%2)?'1':'0';
                    res[i][2*j+1] = (j%2)?'1':'0';
                }
            }
            for(int x=nums_2*2;x<m;x++){
                if(res[i][x-1]=='0') res[i][x]='1';
                if(res[i][x-1]=='1') res[i][x]='0';
            }
        }
    }
    else if( k>=m && !(k % m) ){
        int nums_2 = k/m;
        for(int i=0;i<m;i++){
            for(int j=0;j<nums_2;j++){
                if(i%2){
                    res[2*j][i] = (j%2)?'0':'1';
                    res[2*j+1][i] = (j%2)?'0':'1';
                }else{
                    res[2*j][i] = (j%2)?'1':'0';
                    res[2*j+1][i] = (j%2)?'1':'0';
                }
            }
            for(int x=nums_2*2;x<n;x++){
                if(res[x-1][i]=='0') res[x][i]='1';
                if(res[x-1][i]=='1') res[x][i]='0';
            }
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cout<<res[i][j];
        }
        cout<<endl;
    }
    return 0;
}
