# INF2004 Embedded Systems Project
Welcome to the **Team 55** Car Project repository. This repo contains our source code for our Intelligent Autonomous Robot

## Table of Contents
- [Introduction](#introduction)
- [Getting Started](#getting-started)
- [Architecture Guidelines](#architecture-guidelines)
- [Documentations](#documentations)
- [Branches](#branches)
- [Functions/Methods - Mapping and Pathfinding](#functionsmethods---mapping-and-pathfinding)
- [Functions/Methods - Components](#functionsmethods---components)

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

## Branches

The main branch is the official branch for this project, the other branches are archives. 

## Functions/Methods - Mapping and Pathfinding

### linkedlist.c
1. **insertAtHead**: Inserts a node into the head of the linkedlist
2. **insertAtTail**: Inserts a node into the tail of the linkedlist
3. **remove_at_head**: Removes a node from the head of the linkedlist
4. **removeNode**: Removes a node in a linkedlist
5. **in_ll**: Checks if a node is in a linkedlist
6. **find_mid_node**: Finds the middle node in the linkedlist
7. **size**: Returns the size of the linedlist
8. **isEmpty**: Checks if the linkedlist is empty
9. **ll_init**: Initializes the linkedlist
10. **print_ll**: Prints the content of the linkedlist (A*)

### mapping.c
1. **mapping_task**: FreeRTOS task for mapping
2. **lowest_common_ancestor**: Recursive function, aims to find the intersecting node between 2 paths that lead to the entrance node
3. **generate_path_to_node**: Generates a path from the starting node, to the destination node.
4. **visit_node**: From the car's current position, generate a path to the next node we need to visit.
5. **discover_map**: DFS algorithm

### sorting.c
1. **merge**: Merging function, to merge 2 sorted arrays into 1 array. Uses f-cost as the sorting value. (f-cost is a variable used in A* pathfinding)
2. **merge_sort**: Recursive function which uses divide and conquer to achieve O(n log n) operation to sort a linkedlist

### pathfinding.c
1. **generate_path**: Waits for the mapping algorithm to finish discovering the map, then use A* pathfinding to find the shortest path
2. **sort_f_cost**: Sorts the list by f-cost, uses merge_sort() to accomplish this.
3. **calculate_g_cost**: Part of A* pathfinding parameter, it is the cost it takes to get to this node from the starting node. (cost to get here from previous node + previous node's g-cost)
4. **calculate_h_cost**: Part of A* Pathfinding parameter, the h-cost is the distance from the current node to the goal node. Use pythagoras theorem as an estimate.
5. **compute_path**: A* pathfinding, computes the shortest path to the goal node using f-cost as a deciding factor on which node to visit first (in order to get the fastest computational speed to the goal node)
6. **pathfinding_task**: Waits for the mapping algorithm to finish discovering the map, then use A* pathfinding to find the shortest path
7.** print_pathfinding_grid**: Prints the shortest path from entrance to exit, in the map.

### grid.c
1. **grid_init**: After memory is allocated to the grid, initializes it by initializing every node in the grid as well
2. **print_grid**: Prints the grid of the map
3. **add_wall**: This function is called when a wall needs to be added on the node. If you add a west wall on the node, then the neighbouring left node's east wall also needs to be added.

## Functions/Methods - Components

### interrupt.c
1. **interrupt_callback**: Handles the interrupts for different components

### infrared.c
1. **infrared_task**: Task Used to run the IR Sensor
2. **sensor_data_init**: Sets all the sensor pin to default values
3. **sensor_measure**: Get and increment the count of what the pin is sensing, black or white
4. **ir_sensor_init**: Initialise the pins for IR sensing
5. **ir_sensor_enable**: Add repeating timer to keep sensing for black or white
6. **ir_sensor_disable**: Disable the IR Sensor to stop sensing

### magnetometer.c
1. **magnetometer_task**: VTask to be ran by freeRtos. Initialises the magnometer. Reads the magnometer every 2 seconds depending on the direction facing
2. **i2c_init_pins**: Initializes I2C Pins
3. **magnetometer_init**: Initializes the magnetometer. Further Code explanation in the function
4. **accelerometer_init**: Initializes the accelerometer
5. **reasing_magnetometer**: Reads the x, y and z axis and storing it into the array
6. **reading_accelerometer**: Reads the x, y and z accelerated and storing it into the array
7. **read_angle_difference**:  Reads the difference in angle from the current heading and initial heading
8. **calculate_angle**: Using the Arctangent function formula to calculate the angle

### motor.c
1. **motor_task**: VTask for motor to be ran by freeRtos. Initialises the l298n. Runs anything in the while loop
2. **l298n_speed_pwm_setup**: Set up the PWM pins and the GPIO pins to control the direction using the L298N
3. **set_speed**: Set the speed using the PWM for the car to move. This function makes the car start
4. **set_direction**: Set the Direction of the car base on the DORECTION Enum. Useful for functions using the enum Uses a Swtch case statement
5. **set_forward**: Set the car to run forward by setting the GPIO pins on the controller
6. **set_backward**: Set the car to run backward by setting the GPIO pins on the controller
7. **set_left**: Set the car to turn left by setting the GPIO pins on the controller
8. **set_right**: Set the car to turn right by setting the GPIO pins on the controller
9. **set_stop**: Stops the car by setting the GPIO pins on the controller

### ultrasonic.c
1. **measure_distance**: Measure the distance between the ultrasonic sensor and the object
2. **ultrasonic_init**: Initializes the pins for ultrasonic
3. **measure_ultrasonic**: Reads if there is an obstacle infront of the Ultrasonic. If there is, tell the PID to change to an OBJECT State
4. **ultrasonic_task**: Task used to Run the Ultrasonic

### wheel_encoder.c
1. **wheel_encoder_task**: VTask for wheel encoders to be ran by freeRtos. Initialises the encoders.
2. **wheel_encoder_init**: Initializes the Wheel Encoder Pins. Sets the number of rising edges for each encoder to 0
3. **read_speed**: Reads the speed between the start and the end time. Distance between the time intervals are calculated and added to total distances

### barcode.c
1. **barcode_task**: VTask for IR Sensor Sensing the Barcode to be ran by freeRtos. Task runs and reads the Barcode.
2. **reset_barcode_params**: Reset all parameters and flags of the barcode task
3. **barcode_init**: Initialises all the pins for the Barcode IR Sensor
4. **alarm_callback**: This alarm_callback tells the PID that there is a wall ahead instead of a barcode
5. **check_if_wall**: This functions checks if the Black line detected by the Barcode IR sensor is a Black Wall or a Barcode
6. **decode_barcode**: Decode the barcode base on the Black and White bar array time data when reading the barcode
7. **barcode_to_char**: Convert the Barcode to Character based on the Black and White bar array data from decode_barcode. Coded the logic using the reference from https://en.m.wikipedia.org/wiki/Code_39
8. **read_barcode**: To read the Barcode. The Thickness of the barcode is measured using Time. For Independent Testing.

