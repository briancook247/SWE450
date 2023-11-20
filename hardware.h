/*
Author: Brian Cook
Version: Milestone 2.0
Course: SWE-350
Professor: Mark Reha
Date of Creation: 10/7/2023
My Own Work
*/

#ifndef HARDWARE_H
#define HARDWARE_H
// Functions to initialize the hardware needed
void initializeSwitches();
void initializeButtons();
// 7-Segment-Display functions
void initializeSevenSegDisplay();
static int decimal_bcd(int decimal);
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

#endif