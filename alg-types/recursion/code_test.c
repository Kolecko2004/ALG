#include <stdio.h>


// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    if (list1 == NULL || list2 == NULL) {
        return 0;
    }
    struct ListNode* temp;
    temp = list1->next;
    list1->next = list2;

    mergeTwoLists(list2, temp);
}