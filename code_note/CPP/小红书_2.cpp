#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    int n, m;
    // cin >> n;
    n =3;
    vector<int> numbers = {-5,4,6};
    // for (int i = 0; i < n; i++) {
    //     cin >> numbers[i];
    // }
    string colors;
    // cin >> colors;
    colors = "RBR";
    // cin >> m;
    m = 3;
    vector<int> times = {1,2,3};
    vector<int> actions= {1,2,0};
    int sum = 0;    
    vector<bool> existed(n,false);   // 判断小球在不在袋子里
    vector<int> timestart(n,0);     // 小球放入袋子的时刻
    vector<int> results;
    vector<int> numbers1 = {0,0,0}; // 存放小球当前数值
    for (int i = 0; i < m; i++) {
        int time = times[i];
        int action = actions[i];
        int sum = 0;
        for (int j = 0; j < n; j++) {
            int color = colors[j] == 'R' ? 1 : -1;
            if (color == 1 && existed[j]) {
                numbers1[j] = time -timestart[j]-1 + numbers[j];
                sum += numbers1[j];
            } else if(color == -1 && existed[j]) {
                numbers1[j] = -time +timestart[j]+1 + numbers[j];
                sum += numbers1[j];
            }
        }
        if (action == 0) {
            results.push_back(sum);
        } else {
            int index = abs(action) - 1;
            int color = action > 0 ? 1 : -1;
            if(action>0){
                timestart[index] = i;
                existed[index]=true;
                sum += numbers1[index];
            }else if(action<0){
                existed[index]=false;
                sum -=numbers1[index];
            }
        }
    }
    // 输出结果
    cout << results.size();
    for (int i = 0; i < results.size(); i++) {
        cout << results[i];
    }
    return 0;
}
