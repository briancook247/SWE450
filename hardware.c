/*
Author: Brian Cook
Version: Milestone 3.0
Course: SWE-350
Professor: Mark Reha
Date of Creation: 10/7/2023
My Own Work
*/

#include "hardware.h"
#include "app.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "address_map_arm.h"

void initializeSwitches()
{
  // Initialize Switches
  // Stubbed out for demonstration due to bugs:
  printf("Switches responded and initialized...\n");
}

void initializeButtons()
{
  // Stubbed out for demonstration due to bugs
  printf("Buttons responded and initialized...\n");
}
/*
  Handles the initilization of our 7-segment display.
*/
void initializeSevenSegDisplay()
{
  // Initialize variables for our 7-segment-display
  int fd = -1;
  // Initialize pointer for the lightweight bridge virtual address
  void *LW_virtual;
  // open physical memory
  if ((fd = open_physical(fd)) == -1)
  {
    printf("Error: unable to open /dev/mem for 7-segment display\n");
    return;
  }
  // map memory to the 7-seg-display
  if ((LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
  {
    printf("Error: unable to map physical memory for 7-segment display\n");
    close_physical(fd);
    return;
  }
  // print to console to show us it worked
  printf("Display responded and initialized...\n");
  // Unmap and close for our 7-segment-display
  unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
  close_physical(fd);
}
/*
  Handles the display of the prices within the program to the 7-segment display
  needs currency passed in to work with
*/
void displayCurrencyPrice(Currency currency)
{
  volatile unsigned int *HEX3_HEX0_ptr, *HEX5_HEX4_ptr;
  int fd = -1;
  void *LW_virtual;

  if ((fd = open_physical(fd)) == -1)
  {
    printf("Error: unable to open /dev/mem for display\n");
    return;
  }
  if ((LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
  {
    printf("Error: unable to map physical memory for display\n");
    close_physical(fd);
    return;
  }
  // Documentation has Hex 0-3 and 4-5 pointers, so instead of one pointer we have to split it into 2
  HEX3_HEX0_ptr = (unsigned int *)(LW_virtual + HEX3_HEX0_BASE); // Pointer for HEX0 to HEX3
  HEX5_HEX4_ptr = (unsigned int *)(LW_virtual + HEX5_HEX4_BASE); // Pointer for HEX4 and HEX5
  // Effectively clears all 6 displays
  *HEX3_HEX0_ptr = 0; // Clear HEX0 to HEX3 displays
  *HEX5_HEX4_ptr = 0; // Clear HEX4 and HEX5 displays
  // print price to console
  printf("Currency: %s, Price: %.2f\n", currency.name, currency.price);
  // truncate the price so that we don't print decimals to the display (only fits 6 digits)
  int price = (int)(currency.price); // Convert to integer (truncate decimals)
  //
  unsigned int display_value_low = 0;
  unsigned int display_value_high = 0;

  // Handle lower 4 digits (HEX3 to HEX0)
  for (int i = 0; i < 4; ++i)
  {
    int digit = price % 10;
    // stores our bcd value
    int bcd_digit = decimal_bcd(digit);
    // Sequentially creates our final bcd digit by shifting the bits
    // to their appropriate spot
    bcd_digit = bcd_digit << (i * 8);
    // combines the current digit with the shifted digit to build our final number
    display_value_low = display_value_low | bcd_digit;

    // Replace compound division assignment with separate statements
    price = price / 10;
  }

  // Handle upper 2 digits (HEX5 to HEX4)
  for (int i = 0; i < 2; ++i)
  {
    int digit = price % 10;
    // Replace compound bitwise OR assignment with separate statements
    int bcd_digit = decimal_bcd(digit);
    bcd_digit = bcd_digit << (i * 8);
    display_value_high = display_value_high | bcd_digit;

    // Replace compound division assignment with separate statements
    price = price / 10;
  }
  // Update HEX0 to HEX3 displays (right 4)
  *HEX3_HEX0_ptr = display_value_low;  
  // Update HEX4 and HEX5 displays (left 2)
  *HEX5_HEX4_ptr = display_value_high; 

  unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
  close_physical(fd);
}

// Later implementing a constant refresh to listen for button press
int buttonPress()
{
  // Might call refreshCurrencyPrice() from here if able
  return 0;
}

// Check if a certain switch is high state or low state
int isSwitchToggled()
{
  // Check if the switch is high/low
  return 0;
}

/*
 * Called when refresh button on hardware is pressed
 Takes in our predefined currency struct type
 * gets a new currency price from simulated data and updates the price
 */
void refreshCurrencyPrice(Currency *currency)
{
  // Simulate real data by refreshing the currency price slightly
  currency->price = currency->price + ((rand() % (int)(currency->price / 10)) - 100);
}

/*
 * Input for changing currency is through switches (1-3) currently
 * Call a switch read in here to get the state of the switches
 */
void changeDisplayedCurrency(Currency *currency)
{
  // Change the currently displayed currency
  // Later will check switches to determine which currency to switch to
}

// Open mem  to give access to physical addresses
int open_physical(int fd)
{
  if (fd == -1)
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
    {
      printf("ERROR: could not open \"/dev/mem\"...\n");
      return (-1);
    }
  return fd;
}

// Close mem to give access to physical addresses
void close_physical(int fd)
{
  close(fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base
 */
void *map_physical(int fd, unsigned int base, unsigned int span)
{
  void *virtual_base;

  // Get a mapping from physical addresses to virtual addresses
  virtual_base = mmap(NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
  if (virtual_base == MAP_FAILED)
  {
    printf("ERROR: mmap() failed...\n");
    close(fd);
    return (NULL);
  }
  return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void *virtual_base, unsigned int span)
{
  if (munmap(virtual_base, span) != 0)
  {
    printf("ERROR: munmap() failed...\n");
    return (-1);
  }
  return 0;
}

/*
"poor man's" bcd algorithm to get our hex display values
*/
int decimal_bcd(int decimal)
{

  switch (decimal)
  {

  case 0:
    return 0x3f;
  case 1:
    return 0x06;
  case 2:
    return 0x5b;
  case 3:
    return 0x4f;
  case 4:
    return 0x4f;
  case 5:
    return 0x6d;
  case 6:
    return 0x7d;
  case 7:
    return 0x07;
  case 8:
    return 0x7f;
  case 9:
    return 0x67;
  default:
    return 0xff;
  }
}
