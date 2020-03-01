/*
    Position.cpp
    Author: Stephen Kenyon
    Date: 12/1/19

    Function member definitions for Position class
*/

#include <iostream>
#include <string>
#include "Position.h"
using namespace std;

// default Position constructor
Position::Position(string &symbol, int quantity, double value) : Stock_Symbol(symbol), Stock_Quantity(quantity), Stock_Value(value)
{
    this->next = NULL;
    this->previous = NULL;
}