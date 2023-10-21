#ifndef SORTING_H
#define SORTING_H

#include "linkedlist.h"

struct Node* merge(struct Node* head_node, struct Node* mid_node){
    struct Node* sorted_head_node = NULL;

    if(head_node->f_cost < mid_node->f_cost){
        sorted_head_node = head_node;
        head_node = head_node->next;
    }
    else{
        sorted_head_node = mid_node;
        mid_node = mid_node->next;
    }

    sorted_head_node->next = NULL;

    struct Node* sorted_node = sorted_head_node;
    struct Node* next_node = NULL;

    while (head_node != NULL && mid_node != NULL){
        if (head_node->f_cost < mid_node->f_cost){
            next_node = head_node->next;
            head_node->next = NULL;
            
            sorted_node->next = head_node;
            head_node->prev = sorted_node;
            sorted_node = sorted_node->next;

            head_node = next_node;
        }
        else{
            next_node = mid_node->next;
            mid_node->next = NULL;

            sorted_node->next = mid_node;
            mid_node->prev = sorted_node;
            sorted_node = sorted_node->next;

            mid_node = next_node;
        }
    }
    while(head_node != NULL){
        next_node = head_node->next;
        head_node->next = NULL;

        sorted_node->next = head_node;
        head_node->prev = sorted_node;
        sorted_node = sorted_node->next;
        head_node = next_node;
    }
    while(mid_node != NULL){
        next_node = mid_node->next;
        mid_node->next = NULL;

        sorted_node->next = mid_node;
        mid_node->prev = sorted_node;
        sorted_node = sorted_node->next;
        mid_node = next_node;
    }

    return sorted_head_node;
}

struct Node* merge_sort(struct Node* head_node){

    if(head_node->next == NULL){
        return head_node;
    }

    struct Node* mid_node = find_mid_node(head_node);

    struct Node* left_head = merge_sort(head_node);
    struct Node* right_head = merge_sort(mid_node);

    return merge(left_head, right_head);
}

#endif