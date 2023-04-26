#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
    int T;
    int n;
    cin >> T;
    for( int i =0;i<T;i++){
        cin>>n;
        vector<int> array(n);
        int sum = 0;
        for(int i=0;i<n;i++){
            cin >> array[i];
            sum+=array[i];
        }
        if(sum*2%n!=0){
            cout<< "NO"<<endl;
            continue;
        }
        int sum_two = sum*2/n;
        sort(array.begin(),array.end());
        int a;
        for(a=0;a<n/2;a++){
            if(array[a]+array[n-a-1]!=sum_two){
                cout<<"NO"<<endl;
                break;
            }
        }
        if(a == n/2)
            cout<<"YES"<<endl;
    }
    return 0;
}
