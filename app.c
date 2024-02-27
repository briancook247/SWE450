/*
Author: Brian Cook
Version: Milestone 3.0
Course: SWE-450
Professor: Mark Reha
Date of Creation: 2/26/2024
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

#include "simple_http_client.h"

int main()
{
  // Initialize Hardware Implementation
  initializeSevenSegDisplay();
  initializeSwitches();
  initializeLCD();

  // Now update the LCD with the http response
  char *response = store_http_request("pro-api.coinmarketcap.com", "/v1/cryptocurrency/listings/latest", "3388b957-7571-4ccd-8c6d-d1ade4e83c26");
  updateLCD(response);
  // send http response
  //send_http_request("pro-api.coinmarketcap.com", "/v1/cryptocurrency/listings/latest", "3388b957-7571-4ccd-8c6d-d1ade4e83c26");

  // Initialize array of currency data
  Currency currencies[] = { 
    {"Bitcoin", 14000},
    {"Ethereum", 700},
    {"Ripple", 5009},
    {"Litecoin", 1020},
    {"Bitcoin Cash", 2300},
    {"Cardano", 1323},
    {"NEM", 21221},
    {"Stellar", 22112},
    {"IOTA", 31211},
    {"Dash", 1000},
    {"Testcoin1", 14000},
    {"Testcoin2", 700},
    {"Testcoin3", 5009},
    {"Testcoin4", 1020},
    {"Testcoin5", 2300},
    {"Testcoin6", 1323},
    {"Testcoin7", 21221},
    {"Testcoin8", 22112},
    {"Testcoin9", 31211},
    {"Testcoin10", 1000},
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