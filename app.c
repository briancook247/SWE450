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



int main() {
  // Initialize Hardware Implementation
  // This includes verifying that the hardware is responding and functional
  initializeSevenSegDisplay();
  initializeButtons();
  initializeSwitches();

  // Initialize array of currency data
  Currency currencies[] = {
      // One price at initialization, then upon refresh price is updated by
      // slight deviation
      {"Bitcoin", 50000},
      {"Ethereum", 4000},
      {"Litecoin", 200},
  };
  // For now just 3 currencies,
  // But to dynamically add or remove currencies from the viewable list, we need
  // to keep track of the amount of currencies So we can use sizeof to get the
  // size of the array
  int numCurrencies = sizeof(currencies) / sizeof(currencies[0]);
  // Set the current selected currency to a default 0, which will curently
  // select Bitcoin
  int selectedCurrency = 0;
  bool exit = false;

  // Until the user wants to quit, run the program within this loop
  while (!exit) {
    // Display the menu and listen for user input
    printf("\nCurrency Tracker Menu:\n");
    
    // Display the currently selected currency
    printf("Current Selected Currency: %s\n\n", currencies[selectedCurrency].name);
    
    // This option will later be a constant display on the 7-segment display
    printf("1. Display currency price\n");
    
    // This option will later be triggered by a button press
    printf("2. Refresh currency price\n");
    
    // This option will later be called by a certain button press (via
    // instructions on display)
    printf("3. Switch currency\n");
    
    // This option will be guided through the display eventually as well
    printf("4. Exit\n");
    printf("Enter selection: ");

    // Read in the user input
    int choice;
    scanf("%d", &choice);

    // Switch case for the varying options in the menu
    switch (choice) {
    case 1:
      // Display the price of the currently selected currency
      displayCurrencyPrice(currencies[selectedCurrency]);

      break;
    case 2:
      // Later will check if the button is pressed to refresh the price
      // Pseudocode: if buttonPressed(), refreshCurrencyPrice();
      refreshCurrencyPrice(&currencies[selectedCurrency]);
      printf("Refresh button pressed... Currency price refreshed.\n");
      break;
    case 3:
      // Allow the user to switch to a different currency
      // Each number option represents a switch
      // i.e. Switch 1 (the first switch) will act as a response of "1"
      printf("Select a currency (1-%d):\n1) Bitcoin\n2) Ethereum\n3) "
             "Litecoin\n...",
             numCurrencies);
      // Read in new selected currency
      int newCurrency;
      scanf("%d", &newCurrency);
      // Make sure currency selection exists/ is valid
      if ((newCurrency >= 1) && (newCurrency <= numCurrencies)) {
        // choose currency index (which will be our selected - 1)
        selectedCurrency = newCurrency - 1;
        printf("Now viewing price of %s.\n", currencies[selectedCurrency].name);
      } else {
        // upon invalid selection
        printf("Invalid currency selection.\n");
      }
      break;
    case 4:
      // Exit program by setting exit to true and returning 0 for now
      printf("Exiting Program...\n");
      exit = true;
      return 0;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  }
  return 0;
}
