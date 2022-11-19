#include<iostream>
#include<string>
#include<vector>
using namespace std;
class Solution {
public:
    string minWindow(string s, string t) {
        // chars[i] 记录t中第i个字母当前缺少个数
        vector<int> chars(128,0);
        vector<bool> flag(128,false);
        for(const char c : t){
            flag[c] = true;
            chars[c]++;
        }
        int l = 0;
        int r = 0;
        int ans = 0;
        int min_len = s.size()+1;
        int min_l = l;
        for(r = 0; r < s.size(); r++){
            // s中匹配到t中字符s[r],chars[s[r]]--
            if(flag[s[r]]){
                if(chars[s[r]]>0){    
                    ans++;   
                }
                chars[s[r]]--;
            }

            // 若匹配到所有t中字符串，尝试令l右移
            while( ans == t.size()){
                if(r-l+1 < min_len){
                    min_l = l;
                    min_len = r-l+1;
                }
                if(flag[s[l]]){
                    chars[s[l]]++;
                    if(chars[s[l]]>0){    
                        ans--;   
                    } 
                }
                l++;
            }
        }
        return min_len > s.size()?"":s.substr(min_l,min_len);
    }
};

int main(){
    string s = "ADOBECODEBANC",t = "ABC";
    Solution slt;
    slt.minWindow(s,t);
}