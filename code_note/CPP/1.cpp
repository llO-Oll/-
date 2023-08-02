#include <iostream>



 struct ListNode {
      int val;
      ListNode *next;
      ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
      ListNode(int x, ListNode *next) : val(x), next(next) {}
};
 
class Solution {
    ListNode* reverse(ListNode* head){      
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while(curr){
            ListNode* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    };
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode* temp= new ListNode();
        temp->next = head;
        ListNode* prev = temp;
        for(int i=0;i<left-1;i++){
            prev=prev->next;
        }
        ListNode* tail = prev;
        for(int i=0;i<right-left+1;i++){
            tail=tail->next;
        }
        // 记录子链接左右位置
        ListNode* leftNode = prev->next;
        ListNode* rightNode = tail->next;
        
        //  分割子链表  
        prev->next = nullptr;
        tail->next = nullptr;

        ListNode* newList = reverse(leftNode);

        //  链接
        prev->next = newList;
        ListNode* newTail = newList;
        while(newTail->next){
            newTail=newTail->next;
        }
        newTail->next = rightNode;
        return temp->next;
    }
};