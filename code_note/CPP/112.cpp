#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        int sum=0;
        return dfs(root,sum,targetSum);
    }
    bool dfs(TreeNode* root,int sum,int targetSum){
        if(!root)   return false;
        if(!root->left && !root->right){
            sum+=root->val;
            return sum == targetSum;
        }
        else if(root->left && !root->right){
            return dfs(root->left, sum + root->val, targetSum);
        }else if(!root->left && root->right){
            return dfs(root->right, sum + root->val, targetSum);
        }else{
            return dfs(root->left, sum + root->val, targetSum) || dfs(root->right, sum + root->val, targetSum);
        }
    }
};

int main(){
    Solution s;
    TreeNode* root = new TreeNode(5);

    cout<<s.hasPathSum(root,22);
}