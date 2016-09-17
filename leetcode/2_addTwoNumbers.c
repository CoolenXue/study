#include <stdlib.h>
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
    struct ListNode * ret = NULL;
    int carry = 0;
    struct ListNode *iter1 = l1, *iter2 = l2;

    struct ListNode *tail = NULL, *cur = NULL;
    while(iter1 != NULL && iter2 != NULL){
        cur = (struct ListNode *)malloc(sizeof(struct ListNode));
        cur->val = iter1->val + iter2->val + carry;
        if(cur->val >= 10){
            cur->val -= 10;
            carry = 1;
        }else{
            carry = 0;
        }
        cur->next = NULL;

        if(ret == NULL){
            ret = tail = cur;
        }else{
            tail->next = cur;
            tail = cur;
        }

        iter1 = iter1->next;
        iter2 = iter2->next;
    }
    
    if(iter1 == NULL && iter2 == NULL && carry == 1){
        cur = (struct ListNode *)malloc(sizeof(struct ListNode));
        cur->val = 1;
        cur->next = NULL;
        tail->next = cur;
        tail = cur;
    }

    iter1 = (iter1 == NULL) ? iter2:iter1;
    tail->next = iter1;
    tail = iter1;

    while(carry != 0){
        iter1->val++;
        if(iter1->val == 10){
            iter1->val = 0;
            carry = 1;
        }else{
            carry = 0;
        }
        iter1 = iter1->next;
    }

    return ret;
}

int main(int argc, char * argv[])
{
	struct ListNode * node1 = malloc(sizeof(struct ListNode));
	if(node1 == NULL){
		printf("malloc node1 error\n");
		return 0;
	}
	node1->val = 5;
	struct ListNode * node2 = malloc(sizeof(struct ListNode));
	if(node2 == NULL){
		printf("malloc node2 error\n");
		return 0;
	}
	node2->val = 5;

	struct ListNode * node = addTwoNumbers(node1, node2);
	while(node != NULL){
		printf("[%d]\n", node->val); 
		node = node->next;
	}

	return 0;
}
