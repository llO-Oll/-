#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<pair<int, int>> tasks(n);
    int total_time = 0;    // 最长时间
    int task_start = 1000000;    // 任务起始时间
    for (int i = 0; i < n; i++) {
        cin >> tasks[i].first >> tasks[i].second;
        if(tasks[i].second>total_time){
            total_time=tasks[i].second;
        }
        if(tasks[i].first<task_start){
            task_start= tasks[i].first;
        }
    }
    int energy = 0;
    for (int i = task_start; i <= total_time; i++) {
        int task_in_time = 0;   
        for (int j = 0; j < n; j++) {
            if (tasks[j].first <= i && i <= tasks[j].second) {
                task_in_time++;
            }
        }
        if (task_in_time == 0) {
            energy += 1;
        } else if (task_in_time == 1) {
            energy += 3;
        } else {
            energy += 4;
        }
    }
    cout << energy << endl;
    return 0;
}
