#include <stdio.h>
#include <stdbool.h>
// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

main () {
    struct ListNode* lr = (struct ListNode*)malloc(sizeof(struct ListNode));

    addTwoNumbers();

    return 0;
}


struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    

    lr->val = l1->val + l2->val;

    addTwoNumbers(l1->next, l2->next);

    return 0;
}