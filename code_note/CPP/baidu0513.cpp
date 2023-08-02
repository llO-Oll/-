#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

const int MAXN = 1000005;
int n, m;
int pos[MAXN]; // 存储所有居民和商场的位置
int cnt[MAXN]; // 统计每个商场被多少位居民选择

struct Resident {
    int pos, nearest_shop; // nearest_shop 表示最近的商场的下标
};

int main() {
    cin >> n >> m;
    vector<Resident> residents;
    vector<int> shops;
    for (int i = 0; i < n+m ; i++) {
        int p;
        cin >> p; 
        pos[i] = p;
    }
    for (int i = 0; i < n+m; i++) {
        int p;
        cin >> p;
        if(p==0)
            residents.push_back({pos[i], -1});
        else
            shops.push_back(pos[i]);

    }

    // 对每个居民，找出最近的商场
    int cur_shop = 0;
    for (int i = 0; i < n; i++) {
        while (cur_shop < m && shops[cur_shop] <= residents[i].pos) {
            cur_shop++;
        }
        int dist1 = cur_shop > 0 ? residents[i].pos - shops[cur_shop-1] : INT_MAX;
        int dist2 = cur_shop < m ? shops[cur_shop] - residents[i].pos : INT_MAX;
        residents[i].nearest_shop = dist1 <= dist2 ? cur_shop - 1 : cur_shop;
    }


    // 统计每个商场被多少位居民选择
    for (int i = 0; i < n; i++) {
        int nearest_shop = residents[i].nearest_shop;
        if (nearest_shop != -1) {
            cnt[nearest_shop]++;
        }
    }

    for (int i = 0; i < m; i++) {
        cout << cnt[i] << endl;
    }
    return 0;
}