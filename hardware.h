/*
Author: Brian Cook
Version: Milestone 3.0
Course: SWE-450
Professor: Mark Reha
Date of Creation: 2/26/2024
My Own Work
*/

#include "lcd/terasic_os_includes.h"
#include "lcd/LCD_Lib.h"
#include "lcd/lcd_graphic.h"
#include "lcd/font.h"
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

#ifndef HARDWARE_H
#define HARDWARE_H

#define HW_REGS_BASE (ALT_STM_OFST)
#define HW_REGS_SPAN (0x04000000)
#define HW_REGS_MASK (HW_REGS_SPAN -1)

#define USER_IO_DIR     (0x01000000)


// Functions to initialize the hardware needed
void initializeSwitches();
// 7-Segment-Display functions
void initializeSevenSegDisplay();
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

#endif