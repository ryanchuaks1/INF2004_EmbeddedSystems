/*
 * Author: Leung Wei Jun
 * Description: Merge Sort implementation on C
 */

#ifndef SORTING_H
#define SORTING_H

#include "linkedlist.h"

struct Node* merge(struct Node* head_node, struct Node* mid_node);
struct Node* merge_sort(struct Node* head_node);

#endif