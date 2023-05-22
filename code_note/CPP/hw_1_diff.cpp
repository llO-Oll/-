#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(){
    int n;
    cin >> n;
    int start = 1000001;
    int end = 0;
    vector<int> diff(10005, 0);

    for(int i=0; i<n; i++){
        int a,b;
        cin >> a >> b;
        start = min(start,a);
        end = max(end, b);
        diff[a]++;
        diff[b+1]--;
    }

    vector<int> res( end + 1, 0 );
    res[0] = diff[0];
    for(int i=start; i<=end; i++){
        res[i] = res[i-1] + diff[i];
    }
    int result = 0;
    for(int i = start; i <= end; i++){
        if(res[i] == 0) result += 1;
        else if(res[i] == 1) result += 3;
        else{
            result += 4;
        }
    }
    cout << result << endl;
    return 0;
}