#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /**
     * 
     * @param matrix int整型vector<vector<>> 矩阵
     * @return int整型
     */
    int findTheMinWithColMaxs(vector<vector<int> >& matrix) {
        int M = matrix.size();
        if(M == 0) return 0;
        int N = matrix[0].size();
        if(N == 0) return 0;
        vector<int> max_values;
        for (int j = 0; j < M; ++j) {
            int max_val = matrix[0][j];
            for (int i = 1; i < N; ++i) {
                if (matrix[i][j] > max_val) {
                    max_val = matrix[i][j];
                }
            }
            max_values.push_back(max_val);
        }
    int min_max = *min_element(max_values.begin(), max_values.end());
    return min_max;
    }
};

int main() {
    vector<vector<int>> m = {{1,2},{3,4}};
    Solution s;
    int res = s.findTheMinWithColMaxs(m);
    cout << res;
    return 0;
}
