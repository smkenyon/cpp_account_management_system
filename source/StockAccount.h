/*
    StockAccount.h
    Author: Stephen Kenyon
    Date: 12/1/19

    Headerfile for StockAccount. Inherits from Account.
    StockAccount is a doubly linked list containing "Positions" which is analagous
    to nodes in a doubly linked list. See Positions.h and Positions.cpp for information
    on the data that these contain.
*/

#include <iostream>
#include <string> // we use string to handle stock symbols
#include <fstream> // we use fstream for loading and setting of data in external txt files
#include <random> // use random for choosing a random file to get values for the price from
#include "Account.h"
#include "Position.h"
using namespace std;

#ifndef STOCKACCOUNT_H
#define STOCKACCOUNT_H

class StockAccount : public Account {

    public:
        StockAccount();
        ~StockAccount();

        // List Methods:
        int size() const;
        void addToStart(Position *);
        void printStockAccount();
        bool removeFromStart();
        bool removeFromEnd();
        void removePositionFromAccount(string); // allows user to remove the position based on it's stock symbol
        bool check_if_sorted();
        void bubbleSortPortfolio();
        void swapPositions(Position *, Position *);
        double getPortfolioValue();
        bool checkSymbolInPortfolio(string);

        // File IO Methods:
        void readFromPortfolioFile();  // call on object creation to build portfolio from an existing text file
        double readFromRandomFile(string); // call to get stock price from random text file
        void createPortfolioFile(); // call to create empty Accout file
        void saveToPortfolioFile(); // save the current portfolio to the account file
        void createPortfolioTransactionHistory();
        void saveToPortfolioTransactionHistory(string, string, int, double, double, string);
        void createPortfolioValueFile();
        void savePortfolioValueFile(double, string);

        // TODO: think about how to create the file structure. It seems that there is conflicting info in the spec sheet

        // Menu Utility Methods:
        void buyShares(string, int); // utility function for the buying of shares. User io is handled outside of StockAccount class
        void sellShares(string, int); // similar to buyShares, user io handled outside of StockAccount class
        void graphPortfolioValue();
        void printTransactionHistory();
        int getSize();

    private:
        Position *myHead;
        Position *myTail;
        int mySize;
        int isSorted; // flag to allow to check if the list is sorted
};

#endif
