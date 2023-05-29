#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

int main(){
	int nums;
	cin>>nums;
	vector<int> cost(nums);
	for(int i=0;i<nums;i++){
		cin>>cost[i];
	}
	int cost_sum_1=0;
	int cost_sum_2=0;
	queue<int> q;
	q.push(0);
	int count = 0;
	while(!q.empty()){
		int _size = q.size();
		int sum = 0;
		for(int i=0; i<_size; i++ ){
			int node = q.front();
			q.pop();
			sum += cost[node];
			if((node*2+1)<nums){
				q.push(node*2+1);
			}
			if((node*2+2)<nums){
				q.push(node*2+2);
			}
		}
		if(count%2)
			cost_sum_1+=sum;	
		else
			cost_sum_2+=sum;	
		count++;
	}
	cout << max(cost_sum_1,cost_sum_2);
}