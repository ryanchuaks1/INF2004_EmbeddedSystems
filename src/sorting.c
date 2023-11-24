#include "../include/sorting.h"

/*
 * merge()
 * ---------------------
 * Purpose: Merging function, to merge 2 sorted arrays into 1 array. Uses f-cost as the sorting value. (f-cost is a variable used in A* pathfinding)
 * 1. Determine which starting node is smaller, that will be the starting node of the merged array
 * 2. from there, we have 2 pointers/indexes on both array, compare them, the smaller one is added to the array, and the index + 1 for that array.
 * 3. Repeat 2 until either one of the array is empty.
 * 4. Append all of the values of the leftover array into the merged array.
 * Arguments: starting node of the 1st array, and starting node of the 2nd array
 * Returns: starting node of the merged array
 */
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

    // if both the array still has nodes
    while (head_node != NULL && mid_node != NULL){
        // if array 1's current index variable is smaller than array 2's current index variable,
        // add the variable of array 1 into the merged array, then increment the index/pointer by 1 (next node)
        if (head_node->f_cost < mid_node->f_cost){
            next_node = head_node->next;
            head_node->next = NULL;
            
            sorted_node->next = head_node;
            head_node->prev = sorted_node;
            sorted_node = sorted_node->next;

            head_node = next_node;
        }

        // if array 2's current index is smaller OR equal to array 1's current index variable
        else{
            next_node = mid_node->next;
            mid_node->next = NULL;

            sorted_node->next = mid_node;
            mid_node->prev = sorted_node;
            sorted_node = sorted_node->next;

            mid_node = next_node;
        }
    }

    // these 2 while loops, only 1 of them will activate (because the other one will be empty when it reaches this point).
    // add the rest of the variables in the array to the merged array
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

    // return the starting node of the merged array
    return sorted_head_node;
}

/*
 * merge_sort()
 * ---------------------
 * Purpose: Recursive function which uses divide and conquer to achieve O(n log n) operation to sort a linkedlist
 * 1. Find the mid node of an array, then split it into 2 arrays
 * 2. Repeat 1 for each array, until the array size is 1, then return that
 * 3. Merge both sorted arrays, by sorting them again during the merge
 * 4. Repeat 3 until the array is fully merged back to an array of the original size
 * Arguments: starting node of the 1st array, and starting node of the 2nd array
 * Returns: starting node of the sorted array
 */
struct Node* merge_sort(struct Node* head_node){

    if(head_node->next == NULL){
        return head_node;
    }

    struct Node* mid_node = find_mid_node(head_node);

    struct Node* left_head = merge_sort(head_node);
    struct Node* right_head = merge_sort(mid_node);

    return merge(left_head, right_head);
}