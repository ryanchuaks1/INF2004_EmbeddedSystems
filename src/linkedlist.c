/*
 * Author: Wei Jun
 * Description: LinkedList related methods
 */

#include "../include/mapping.h"

/*
 * insertAtHead()
 * ---------------------
 * Purpose: Inserts a node at the head of the linkedlist
 * Arguments: LinkedList and the new node to be added
 * Returns: void
 */
void insertAtHead(struct LinkedList* ll, struct Node* newNode){
    struct Node* currHead = (*ll).head;
    if(currHead != NULL){
        (*currHead).prev = newNode;
        (*newNode).next = currHead;
    }
    else{
        (*ll).tail = newNode;
    }
    (*ll).head = newNode; //get the actual LL struct, then store the struct Node address value into the head variable.
}

/*
 * insertAtTail()
 * ---------------------
 * Purpose: Inserts a node at the tail of the linkedlist
 * Arguments: LinkedList and the new node to be added
 * Returns: void
 */
void insertAtTail(struct LinkedList* ll, struct Node* newNode){
    struct Node* currTail = (*ll).tail;
    if(currTail != NULL){
        (*currTail).next = newNode;
        (*newNode).prev = currTail;
    }
    else{
        (*ll).head = newNode;
    }
    (*ll).tail = newNode;
}

/*
 * remove_at_head()
 * ---------------------
 * Purpose: Remove the head node in the linkedlist
 * Arguments: LinkedList
 * Returns: void
 */
struct Node* remove_at_head(struct LinkedList* ll){
    struct Node* removed_node = ll->head;

    ll->head = removed_node->next;

    if(ll->head != NULL){
        ll->head->prev = NULL;
        removed_node->next = NULL;
    }
    else{
        ll->tail = NULL;
    }

    return removed_node;
}

/*
 * removeNode()
 * ---------------------
 * Purpose: Remove a node in the LinkedList, useful if you know that this node exist in the LL and you wish to remove it.
 * Arguments: LinkedList, and the node to be removed
 * Returns: void
 */
void removeNode(struct LinkedList* ll, struct Node* node){
    struct Node *prevNode = (*node).prev;
    struct Node *nextNode = (*node).next;

    if(prevNode != NULL){
        (*prevNode).next = nextNode;
    }
    else{
        (*ll).head = nextNode;
    }

    if(nextNode != NULL){
        (*nextNode).prev = prevNode;
    }
    else{
        (*ll).tail = prevNode;
    }
    
    (*node).next = NULL;
    (*node).prev = NULL;
}

/*
 * in_ll()
 * ---------------------
 * Purpose: Checks if a node is in the linkedlist. Does this by traversing the linkedlist using both head and tail pointers (until they meet in the middle if the node doesnt exist)
 * Arguments: LinkedList, and the node to be checked
 * Returns: True = In linkedlist, False = Not in linkedlist
 */
bool in_ll(struct LinkedList* ll, struct Node* node){
    struct Node* head_node = ll->head;
    struct Node* tail_node = ll->tail;

    if(head_node == node || tail_node == node){
        return true;
    }

    while (head_node != tail_node){
        head_node = head_node->next;
        tail_node = tail_node->prev;

        if(head_node == node || tail_node == node){
            return true;
        }
    }

    return false;
}

// struct Node* findAt(struct LinkedList* ll, int index){
//     if(index >= size(ll)){
//         return NULL;
//     }
//     else{
//         int counter = 0;
//         struct Node* curr_node = (*ll).head;
//         while(curr_node != NULL){
//             if(index == counter){
//                 return curr_node;
//             }
//             else{
//                 curr_node = (*curr_node).next;
//                 counter++;
//             }
//         }
//     }

// }

/*
 * find_mid_node()
 * ---------------------
 * Purpose: Finds the middle node in the linked list, used for sorting algorithm (merge sort). Uses O(0.5N) operation by moving 2 pointers.
 * Arguments: Head node of the linkedlist
 * Returns: The address of the middle node
 */
struct Node* find_mid_node(struct Node* head_node){
    struct Node* mid_node = head_node;
    struct Node* end_node = head_node;
    int counter = 0;

    while(mid_node->next != NULL && end_node->next != NULL){
        if(end_node->next->next != NULL){
            mid_node = mid_node->next;
            end_node = end_node->next->next;
        }
        else{ // if it's an even number
            mid_node = mid_node->next;
            break;
        }
    }
    
    mid_node->prev->next = NULL;
    mid_node->prev = NULL;

    return mid_node;
}

/*
 * size()
 * ---------------------
 * Purpose: Returns the size of the linkedlist
 * Arguments: LinkedList
 * Returns: size (int)
 */
int size(struct LinkedList* ll){
    int counter = 0;
    struct Node* currNode = (*ll).head;
    while(currNode != NULL){
        counter++;
        currNode = (*currNode).next;
    }
    return counter;
}

/*
 * isEmpty()
 * ---------------------
 * Purpose: Checks if a linkedlist is empty
 * Arguments: LinkedList
 * Returns: true if empty, false if not empty
 */
bool isEmpty(struct LinkedList* ll){
    return (*ll).head == NULL;
}

/*
 * ll_init()
 * ---------------------
 * Purpose: Used after malloc(), in order to clear and set all variables to default values, to prevent seg fault.
 * Arguments: The linked list
 * Returns: void
 */
void ll_init(struct LinkedList* ll){
    if(ll == NULL){
        printf("Malloc failed. Terminating program...\n");
        exit(1);
    }
    else{
        (*ll).head = NULL;
        (*ll).tail = NULL;
    }
}

/*
 * print_ll()
 * ---------------------
 * Purpose: Prints the content of the linkedlist (for A*)
 * Arguments: LinkedList
 * Returns: void
 */
void print_ll(struct LinkedList* ll){
    struct Node* curr_node = ll->head;
    int counter = 1;
    while (curr_node != NULL){
        printf("Node %d, f_cost=%.2f g_cost=%d h_cost=%.2f, location=(%d,%d)\n", counter, curr_node->f_cost, curr_node->g_cost, curr_node->h_cost, curr_node->location.x, curr_node->location.y);
        curr_node = curr_node->next;
        counter++;
    }
}