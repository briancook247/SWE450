/*
Author: Brian Cook
Version: Milestone 2.0
Course: SWE-350
Professor: Mark Reha
Date of Creation: 10/7/2023
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
