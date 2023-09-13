#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<pair<string, int>> units(n);
    vector<char> survival_status(n, 'Y');
    
    for (int i = 0; i < n; ++i) {
        cin >> units[i].first >> units[i].second;
    }
    
    for (int i = 0; i < m; ++i) {
        int u, v;
        char c1, c2;
        cin >> u >> v >> c1 >> c2;
        --u; --v;  // Convert to 0-based index
        /*
            1. c1是人，选择亮身份,
                a. c2是兽，选择亮身份，发生攻击，
                    c1>c2 c2=N
                b. c2是兽，选择不亮身份，发生攻击，同情况a
                c. c2是人 无事发生
            2. c1是人，选择不亮身份
                a. c2是兽，选择亮身份, 且战斗力小于人 ，c2死
            3. c1是兽，选择不亮身份
                a. c2是人，选择亮身份，发生攻击，
            4. c1是兽，选择亮身份
                a. c2是人，选择亮身份,发生攻击
                b. c2是人，选择不亮身份,只有大于攻击力时发生攻击
        */
       if(survival_status[u]=='Y' && survival_status[v]=='Y'){
            if (c1 == 'Y' && units[u].first == "human") {
                if ( units[v].first == "monster") {
                    if(units[u].second > units[v].second)
                        survival_status[v] = 'N';
                    else if(units[u].second < units[v].second){
                        survival_status[u] = 'N';
                    }
                    else{
                        survival_status[u] = 'N';
                        survival_status[v] = 'N';
                    }
                }
            }
            else if(c1 == 'N' && units[u].first == "human"){
                if(c2 == 'Y' && units[v].first == "monster" ){
                    if(units[u].second > units[v].second){
                        survival_status[v] = 'N';
                    }
                }
            }
            else if (c1 == 'N' && units[u].first == "monster") {
                if (c2 == 'Y' && units[v].first == "human") {
                    if(units[u] > units[v]){
                        survival_status[v] = 'N';
                    }else if(units[u] < units[v]){
                        survival_status[u] = 'N';
                    }else{
                        survival_status[u] = 'N';
                        survival_status[v] = 'N';
                    }
                    
                } 
            }
            else if( c1 == 'Y' && units[u].first == "monster" ){
                if(c2 == 'Y' && units[v].first == "human"){
                    if(units[u] > units[v]){
                        survival_status[v] = 'N';
                    }else if(units[u] < units[v]){
                        survival_status[u] = 'N';
                    }else{
                        survival_status[u] = 'N';
                        survival_status[v] = 'N';
                    }
                }else if(c2 == 'N' && units[v].first == "human"){
                    if(units[v] > units[u]){
                        survival_status[u] = 'N';
                    }
                }
            }
        }
    }    
    for (char status : survival_status) {
        cout << status;
    }
    return 0;
}
