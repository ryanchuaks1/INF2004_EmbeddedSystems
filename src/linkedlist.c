#include "../include/mapping.h"

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

int size(struct LinkedList* ll){
    int counter = 0;
    struct Node* currNode = (*ll).head;
    while(currNode != NULL){
        counter++;
        currNode = (*currNode).next;
    }
    return counter;
}

bool isEmpty(struct LinkedList* ll){
    return (*ll).head == NULL;
}

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

void print_ll(struct LinkedList* ll){
    struct Node* curr_node = ll->head;
    int counter = 1;
    while (curr_node != NULL){
        printf("Node %d, f_cost=%.2f g_cost=%d h_cost=%.2f, location=(%d,%d)\n", counter, curr_node->f_cost, curr_node->g_cost, curr_node->h_cost, curr_node->location.x, curr_node->location.y);
        curr_node = curr_node->next;
        counter++;
    }
}