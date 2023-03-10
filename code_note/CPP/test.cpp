#define _GLIBCXX_USE_CXX11_ABI 0
#include<iostream>
#include<vector>
#include<deque>
#include<string>
#include<algorithm>
#include<set>
#include <functional>
#include <iterator>
using namespace std;
class Solution{
public:
    vector<string> braceExpansionII(string expression){
        vector<char> op;    // 运算符栈
        vector<set<string>> stk;    // 字符栈
        auto ope = [&](){
            int l = stk.size()-2,r = stk.size()-1;
            // 栈顶是乘法先算乘法
            if(op.back()=='+'){
                stk[l].insert(stk[r].begin(),stk[r].end());
            }else{
                set<string> tmp;
                for(auto &i:stk[l]){
                    for(auto &j:stk[r]){
                        tmp.insert(i+j);
                    }
                }
                stk[l]=move(tmp);
            }
            op.pop_back();
            stk.pop_back();
        };

        for(int i = 0;i<expression.size();i++){
            // 如果当前是','
            if(expression[i] == ','){
                // 当栈顶 不为* 或者 为空 时，再放入+
                while(op.size() && op.back() == '*'){
                    ope();
                }
                op.push_back('+');
            
            } else if(expression[i] == '{' ){
                // 如果当前是'{'
                if(i>0 && (expression[i-1] == '}'|| isalpha(expression[i-1])) ){
                    op.push_back('*');
                }
                op.push_back('{');
            }else if(expression[i] == '}'){
                // 如果当前是'}' 弹出运算符,直到栈顶为{
                while(op.size() && op.back() !='{'){
                    ope();
                }
                op.pop_back();
            }else{
                // 首先判断是否需要添加乘号，再将新构造的集合添加到集合栈中
                if(i > 0 && (expression[i-1] == '}'|| isalpha(expression[i-1]) )){
                    op.push_back('*');
                }
                stk.push_back({string(1,expression[i])});
            }
        }
        while(op.size()){
            ope();
        }
        return {stk.back().begin(),stk.back().end()};
    }

};
int main(){
    string s = "{a,b}{c,{d,e}}";
    Solution slt;
    slt.braceExpansionII(s);
}