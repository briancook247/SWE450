/*
Author: Brian Cook
Version: Milestone 2.0
Course: SWE-350
Professor: Mark Reha
Date of Creation: 10/7/2023
My Own Work
*/

#include "hardware.h"
#include "app.h"
#include <stdio.h>

void initializeSwitches() {
  // Initialize Switches
  // Stubbed out version:
  printf("Switches responded and initialized...\n");
}

void initializeSevenSegDisplay(){
  // Initialize 7-segment display
  printf("Display responded and initialized...\n");
}

void initializeButtons() {
  // Initialize Buttons needed
  printf("Buttons responded and initialized...\n");
}

/*
 * Takes in our predefined currency struct type
 * displays the currency's price to the 7-segment display
 * For now displays both name and price to console
 */
void displayCurrencyPrice(Currency currency) {
  // Display the currency price on the 7-segment display
  printf("Currency: %s, Price: %.2f\n", currency.name, currency.price);
}

// Later implementing a constant refresh to listen for button press
int buttonPress() {
  // Might call refreshCurrencyPrice() from here if able
  return 0;
}

// Check if a certain switch is high state or low state
int isSwitchToggled() {
  // Check if the switch is high/low
  return 0;
}

/*
 * Called when refresh button on hardware is pressed
 Takes in our predefined currency struct type
 * gets a new currency price from simulated data and updates the price
 */
void refreshCurrencyPrice(Currency *currency) {
  // Simulate real data by refreshing the currency price slightly
  currency->price = currency->price + ((rand() % (int)(currency->price/10)) - 100);
}

/*
  * Input for changing currency is through switches (1-3) currently
  * Call a switch read in here to get the state of the switches
  */
void changeDisplayedCurrency(Currency *currency) {
  // Change the currently displayed currency
  // Later will check switches to determine which currency to switch to
}
