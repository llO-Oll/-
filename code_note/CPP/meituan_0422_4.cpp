#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    string s;
    cin >> s;
    int n = s.size();

    
    vector<int> dp1(n+1, 0);
    vector<int> dp3(n+1, 0);
    for (int i = 1; i <= n; i++) {
        // dp1[i]表示删除后缀[i, n-1]后，剩余字符串中0的个数
        dp1[i] = dp1[i-1] + (s[i-1] == '0' ? 1 : 0);
        // dp3[i]表示删除前缀[0, i-1]后，删除字符串中1的个数
        dp3[i] = dp1[i-1] + (s[i-1] == '1' ? 1 : 0);
    }

    
    vector<int> dp2(n+1, 0);
    vector<int> dp4(n+1, 0);
    for (int i = n-1; i >= 0; i--) {
        // dp2[i]表示删除后缀[i, n-1]后，删除字符串中1的个数
        dp2[i] = dp2[i+1] + (s[i] == '1' ? 1 : 0);
        // dp4[i]表示删除前缀[0, i-1]后，剩余字符串中0的个数
        dp4[i] = dp4[i+1] + (s[i] == '0' ? 1 : 0);
    }

    int ans = min(dp1[n], dp2[0]);
    for (int i = 0; i <= n; i++) {
        ans = min(ans, dp1[i] + dp2[i]);
    }
    cout << ans << endl;
    return 0;
}
