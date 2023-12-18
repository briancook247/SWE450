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

// GPIO Bit Structure, 6 nibbles set up for Hex0 – Hex 5 BCD Decoder Inputs
typedef struct {
    unsigned int gpio0 : 4;  // lsb
    unsigned int gpio1 : 4;
    unsigned int gpio2 : 4;
    unsigned int gpio3 : 4;
    unsigned int gpio4 : 4;
    unsigned int gpio5 : 4;
    unsigned int gpiou : 11; // msb
} GpioRegister;

// virtual address pointer to JP1 Expansion Port
volatile unsigned int *JP1_ptr;
GpioRegister* gpioRegister;

void initializeSwitches()
{
  // Initialize Switches

  int fd = -1;
  void *LW_virtual;

  // Open physical memory
  if ((fd = open_physical(fd)) == -1)
  {
    printf("Error: unable to open /dev/mem for switches\n");
    return;
  }

  // Map memory for switch access
  if ((LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
  {
    printf("Error: unable to map physical memory for switches\n");
    close_physical(fd);
    return;
  }

  // Get the switch address
  volatile unsigned int *SWITCH_ptr = (volatile unsigned int *)(LW_virtual + SW_BASE);

  // Read switch state
  unsigned int switchState = *SWITCH_ptr;

  // Respond to switch state
  for (int i = 0; i < 10; ++i) { // Assuming 10 switches for this example
    if (switchState & (1 << i)) {
      printf("Switch %d is ON\n", i);
    } else {
      printf("Switch %d is OFF\n", i);
    }
  }


  printf("Switches responded and initialized...\n");

  // Unmap and close physical memory
  unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
  close_physical(fd);
}
/*
* Handle switching the currency to be displayed
* takes in the list of currencies, number of currencies, and the currency selected currency
*/
void switchDisplayedCurrency(Currency *currencies, int numCurrencies, int *selectedCurrency)
{
  int fd = -1;
  void *LW_virtual;
  volatile unsigned int *SWITCH_ptr; // pointer to switches
  unsigned int switchState; // temp var to hold state of the switches

  // Open the physical memory file for reading switch states
  if ((fd = open_physical(fd)) == -1) {
    printf("Error: unable to open /dev/mem for switches\n");
    return;
  }
  // Map the physical memory for switches into virtual memory
  if ((LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL) {
    printf("Error: unable to map physical memory for switches\n");
    close_physical(fd);
    return;
  }
  // Find the address of the switch and read the state
  SWITCH_ptr = (volatile unsigned int *)(LW_virtual + SW_BASE);
  // store in switchState
  switchState = *SWITCH_ptr;
  // Go through the switches to check if a switch corresponding with a currency
  // is switched to the high (ON) state and make it the new selected currency
  for (int i = 0; i < numCurrencies; ++i) {
    if (switchState & (1 << i)) {

      *selectedCurrency = i;
      // notify user of the currency switch
      printf("Currency switched to: %s\n", currencies[*selectedCurrency].name);
      // exit when we switch to the desired currency
      break;
    }
  }

  // Unmap and close physical memory
  unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
  close_physical(fd);
}


/*
  Handles the initilization of our 7-segment display.
*/
void initializeSevenSegDisplay()
{
    int fd = -1;
    void *LW_virtual;

    if ((fd = open_physical(fd)) == -1)
    {
        printf("Error: unable to open /dev/mem for 7-segment display\n");
        return;
    }

    if ((LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
    {
        printf("Error: unable to map physical memory for 7-segment display\n");
        close_physical(fd);
        return;
    }

    JP1_ptr = (unsigned int *)(LW_virtual + JP1_BASE);
    // Set all GPIO to output
    *(JP1_ptr + 1) = 0x00FFFFFF;
    // Set the gpioRegister pointer
    gpioRegister = (GpioRegister*)(JP1_ptr + 0); 

    printf("7-segment display initialized...\n");

    unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
    close_physical(fd);
}

/*
  Handles the display of the prices within the program to the 7-segment display
  needs currency passed in to work with
*/
void displayCurrencyPrice(Currency currency)
{
    int fd = open_physical(-1);
    if (fd == -1) return;

    void *LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    if (LW_virtual == NULL) {
        close_physical(fd);
        return;
    }

    JP1_ptr = (unsigned int *)(LW_virtual + JP1_BASE);
    // Set the gpioRegister pointer
    gpioRegister = (GpioRegister*)(JP1_ptr + 0); 

    int price = (int)(currency.price);

    // Store the digits in an array in reverse order
    int digits[6];
    for (int i = 0; i < 6; i++) {
        digits[i] = price % 10;
        price /= 10;
    }

    // Assign each digit to the corresponding GPIO bit for the 7-segment display
    gpioRegister->gpio5 = digits[5];
    gpioRegister->gpio4 = digits[4];
    gpioRegister->gpio3 = digits[3];
    gpioRegister->gpio2 = digits[2];
    gpioRegister->gpio1 = digits[1];
    gpioRegister->gpio0 = digits[0];

    printf("Displaying currency: %s, Price: %d\n", currency.name, (int)(currency.price));

    unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
    close_physical(fd);
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
