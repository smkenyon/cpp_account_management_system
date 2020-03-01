/*
    Position.h
    Author: Stephen Kenyon
    Date: 12/1/19

    Header file for position class. Each Position contains information for the stock symbol and the quantity held. The 
    value of the position is calculated externally to the class. In addition, to allow for the creation of a doubly 
    linked list, Position contains a next and previous pointer. StockAccount is a friend class that implements
    the doubly linked list.
*/

#include <iostream>
#include <string>
using namespace std;

#ifndef POSITION_KENYON_H
#define POSITION_KENYON_H

class Position { 

    friend class StockAccount;

    public:
        Position(string &symbol, int quantity, double value);

    private:
        string Stock_Symbol;
        int Stock_Quantity;
        double Stock_Value;
        Position *next;  // We have a next and previous pointer to allow for doubly linked list
        Position *previous;

};

#endif
