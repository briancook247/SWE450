/*
Author: Brian Cook
Version: Milestone 3.0
Course: SWE-350
Professor: Mark Reha
Date of Creation: 10/7/2023
My Own Work
*/

#include "app.h"
#include "hardware.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "address_map_arm.h"

int main()
{
  // Initialize Hardware Implementation
  initializeSevenSegDisplay();
  initializeSwitches();

  // Initialize array of currency data
  Currency currencies[] = {
      {"Bitcoin", 50000},
      {"Ethereum", 4000},
      {"Litecoin", 2000},
  };

  // Calculate number of currencies for indexing (dynamic so we can add more later)
  int numCurrencies = sizeof(currencies) / sizeof(currencies[0]);

  // Index for our currently selected currency to simulate
  int selectedCurrency = 0;

  // As long as the board is on, this will run
  while (1)
  { // Infinite loop
    // Check and update currency based on switch state
    switchDisplayedCurrency(currencies, numCurrencies, &selectedCurrency);

    // Refresh and display new prices constantly
    refreshCurrencyPrice(&currencies[selectedCurrency]);
    displayCurrencyPrice(currencies[selectedCurrency]);

    // Make the program wait for a second to prevent updating the price too often
    sleep(1);
  }

  return 0;
}
