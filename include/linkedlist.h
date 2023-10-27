/*
 * Author: Leung Wei Jun
 * Description: LinkedList implementation on C
 * Functions/Methods:
 * 1. insertAtHead
 * 2. insertAtTail
 * 3. remove_at_head
 * 4. removeNode
 * 5. in_ll
 * 6. find_mid_node
 * 7. size
 * 8. isEmpty
 * 9. ll_init
 * 10. print_ll
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "common.h"
#include "node.h"

struct LinkedList{
    struct Node *head;
    struct Node *tail;
};

// struct Node{
//     struct Node *prev;
//     struct Node *next;
//     struct Node* value;
// };

void ll_init(struct LinkedList* ll);
void insertAtHead(struct LinkedList* ll, struct Node* newNode);
void insertAtTail(struct LinkedList* ll, struct Node* newNode);
struct Node* remove_at_head(struct LinkedList* ll);
void removeNode(struct LinkedList* ll, struct Node* node);
bool in_ll(struct LinkedList* ll, struct Node* node);
struct Node* find_mid_node(struct Node* head_node);
int size(struct LinkedList* ll);
bool isEmpty(struct LinkedList* ll);
void print_ll(struct LinkedList* ll);



#endif