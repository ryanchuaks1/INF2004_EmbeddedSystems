# INF2004 Embedded Systems Project
Welcome to the **Team 55** Car Project repository. This repo contains our source code for our Intelligent Autonomous Robot

## Table of Contents
- [Introduction](#introduction)
- [Getting Started](#getting-started)

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

The brain/main of the project is in main.c file. The project is running under FreeRTOS and hence, most of the components are seperated into task. Each component has thier own file.    <br>     
Task with the same priority does round robin however, higher priority task will always take the CPU first. Interrupts are always handled first before task.
