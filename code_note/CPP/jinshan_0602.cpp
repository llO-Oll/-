#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

int dfs(int root,vector<int>& tree, bool& result){
    if(2*root+1>tree.size()-1){
        return tree[root];
    }
    int left = 2*root+1;
    int right = 2*root+2;
    int leftsum = dfs(left,tree,result);
    int rightsum = dfs(right,tree,result);
    if(leftsum == rightsum ){
        result=true;
    }
    return tree[root] + leftsum + rightsum;
}

int main(){
	int nums;
	cin>>nums;
    int a;
	vector<vector<int>> tree(nums,vector<int>());
	for(int i=0;i<nums;i++){
		while(cin>>a){
			if (cin.get() == '\n')
				break;
            tree[i].push_back(a);
        }
	}

    for(int i=0;i<nums;i++){
        bool result = false;
        dfs(0,tree[i],result);
        cout<<result;
    }
    return 0;

}