/*
Author: Brian Cook
Version: Milestone 3.0
Course: SWE-450
Professor: Mark Reha
Date of Creation: 2/26/2024
My Own Work
*/

#ifndef APP_H
#define APP_H

typedef struct {
  // Make a char array to act as a string for the currency name
  char name[20];
  // Make a double member called price, to hold the price of the currency.
  double price;
} Currency;

#endif
