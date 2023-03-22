#include <iostream>
//需要引入 vector 头文件
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

class CQueue {
    stack<int> q1,q2;
public:
    CQueue(){
    }
    
    void appendTail(int value) {
        q1.push(value);
    }
    
    int deleteHead() {
        if(q2.empty()){
            while(!q1.empty()){
                q2.push(q1.top());
                q1.pop();
            }

        }
        int head = q2.top();
        q2.pop();
        return head;
    }
};

/**
 * Your CQueue object will be instantiated and called as such:
 * CQueue* obj = new CQueue();
 * obj->appendTail(value);
 * int param_2 = obj->deleteHead();
 */
int main()
{
    CQueue* obj = new CQueue();
    int x = -2;
    
    int y = x>>1;
    cout<< y <<endl;
    return 0;
}
