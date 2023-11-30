# INF2004 Embedded Systems Project
Welcome to the **Team 55** Car Project repository. This repo contains our source code for our Intelligent Autonomous Robot

## Table of Contents
- [Introduction](#introduction)
- [Getting Started](#getting-started)
- [Architecture Guidelines](#architecture-guidelines)
- [Documentations](#documentations)
- [Branches](#branches)

## Introduction

This project involves an intelligent autonomous 2-wheel robot car using the  Raspberry Pi Pico microcontroller, equipped  with  various  sensors  and a PID (Proportional-Integral-Derivative) controller. The primary goal is to develop a robot car that navigates a predefined track, efficiently  avoiding obstacles and recognizing barcodes using infrared sensors. The integration of  a PID controller enhances the car's control system, ensuring smooth and accurate motion during navigation and barcode scanning.

## Installation Guide

This project is designed to be built using Pico Visual Studio Code, a development environment optimized for Raspberry Pi Pico projects. If you haven't already, [download and install Pico Visual Studio Code](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html#raspberry-pi-pico-cc-sdk) on your computer.

## Getting Started

To get started with this car project, follow these steps: 

1.  **Clone the Repository**: Begin by cloning this repository to your local machine
    
2.  **Open the Project in Pico VS Code**: Launch Pico Visual Studio Code, and use the "File" menu to open the cloned repository folder (`car-project`).

3.  **Add the FreeRTOS Kernal from GitHub**: Run **"git submodule update --init"** to pull the FreeRTOS Kernal into the Project
    
4.  **Build the Project**: With the project open, navigate to the build menu or use the keyboard shortcut to build the project. This will compile the code and generate the necessary binary files.
    
5.  **Upload to Raspberry Pi Pico**: After building the project, you can flash the generated uf2 file onto your Raspberry Pi Pico. Connect your Pico to your computer while pressing the white button and copy the uf2 file in.
    
6.  **Run the Project**: Once the code is uploaded to your Pico, you can *run* the car project and observe its behavior.

## Architecture Guidelines

The brain/main of the project is in main.c file. The project is running under FreeRTOS and hence, most of the components are seperated into task. Each component has their own file.    <br>     
Task with the same priority does round robin however, higher priority task will always take the CPU first. Interrupts are always handled first before task.

## Documentations

The Proposals, Block Diagram and Flow Chars are in the "diagrams_and_proposal" Folder.

## Functions/Methods

linkedlist.c
1. insertAtHead: Inserts a node into the head of the linkedlist
2. insertAtTail: Inserts a node into the tail of the linkedlist
3. remove_at_head: Removes a node from the head of the linkedlist
4. removeNode: Removes a node in a linkedlist
5. in_ll: Checks if a node is in a linkedlist
6. find_mid_node: Finds the middle node in the linkedlist
7. size: Returns the size of the linedlist
8. isEmpty: Checks if the linkedlist is empty
9. ll_init: Initializes the linkedlist
10. print_ll: Prints the content of the linkedlist (A*)

mapping.c
1. mapping_task: FreeRTOS task for mapping
2. lowest_common_ancestor: Recursive function, aims to find the intersecting node between 2 paths that lead to the entrance node
3. generate_path_to_node: Generates a path from the starting node, to the destination node.
4. visit_node: From the car's current position, generate a path to the next node we need to visit.
5. discover_map: DFS algorithm

sorting.c
1. merge: Merging function, to merge 2 sorted arrays into 1 array. Uses f-cost as the sorting value. (f-cost is a variable used in A* pathfinding)
2. merge_sort: Recursive function which uses divide and conquer to achieve O(n log n) operation to sort a linkedlist

pathfinding.c
1. generate_path: Waits for the mapping algorithm to finish discovering the map, then use A* pathfinding to find the shortest path
2. sort_f_cost: Sorts the list by f-cost, uses merge_sort() to accomplish this.
3. calculate_g_cost: Part of A* pathfinding parameter, it is the cost it takes to get to this node from the starting node. (cost to get here from previous node + previous node's g-cost)
4. calculate_h_cost: Part of A* Pathfinding parameter, the h-cost is the distance from the current node to the goal node. Use pythagoras theorem as an estimate.
5. compute_path: A* pathfinding, computes the shortest path to the goal node using f-cost as a deciding factor on which node to visit first (in order to get the fastest computational speed to the goal node)
6. pathfinding_task: Waits for the mapping algorithm to finish discovering the map, then use A* pathfinding to find the shortest path
7. print_pathfinding_grid: Prints the shortest path from entrance to exit, in the map.

grid.c
1. grid_init: After memory is allocated to the grid, initializes it by initializing every node in the grid as well
2. print_grid: Prints the grid of the map
3. add_wall: This function is called when a wall needs to be added on the node. If you add a west wall on the node, then the neighbouring left node's east wall also needs to be added.

## Branches

The main branch is the official branch for this project, the other branches are archives. 
