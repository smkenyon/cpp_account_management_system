/*
    StockAccount.cpp
    Author: Stephen Kenyon
    Date: 12/1/19

    Member function defintions for StockAccount class.
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <new>
#include <string>
#include "StockAccount_Kenyon.h"

// matlab includes

#include <yvals.h>
#if (_MSC_VER >= 1600)
#define __STDC_UTF_16__
#endif
#include "mex.h"
#include <engine.h>
#pragma comment(lib, "libmx.lib")
#pragma comment(lib, "libmex.lib")
#pragma comment(lib, "libeng.lib")


using namespace std;

StockAccount::StockAccount() : Account()
{ /*
    Default constructor first sets myHead, myTail, and mySize to 0 for an empty list. 
    Then, checks to see if portfolio.txt exists. If not, it creates portfolio.txt.
    If portfolio.txt does exist, the list is constructed from the values in portfolio.txt 
    and sorted.
  */

    ifstream portfolio_file;
    portfolio_file.open("portfolio.txt");
    if (!portfolio_file.good()) // if we don't have a file, then initalize an empty list
    {
        // initalize the first and last node to 0 for empty list and set size to 0
        myHead = 0;
        myTail = 0;
        mySize = 0;
        createPortfolioFile();
    }
    else // read portfolio.txt and build list from contents in file
    {
        myHead = 0;
        myTail = 0;
        mySize = 0;
        readFromPortfolioFile();
        if (!check_if_sorted())
            bubbleSortPortfolio();
    }
}

StockAccount::~StockAccount()
{
    // check to make sure list is not empty
    if (size() > 0)
    {
        Position *currentPtr = myHead; // set active position node to head
        Position *tempPtr; // use for swap

        // delete all the remaining position nodes, incrementing to the right from head
        while (currentPtr != 0) // if at null pointer, finished
        {
            tempPtr = currentPtr; // swap
            currentPtr = currentPtr->next; // go to the next position
            delete tempPtr; // remove the old position
        }
    }
}

int StockAccount::size() const
{
    return mySize;
}

void StockAccount::addToStart(Position *p)
{
    Position *newPtr = new Position(p->Stock_Symbol, p->Stock_Quantity, p->Stock_Value); // create a new position with stock symbol and quantity
    
    // first check if list is empty
    if (size() == 0)
        myHead = myTail = newPtr; // nothing in list, so new position is the head and tail
    else
    {
        newPtr->next = myHead; // set newPtr's next to head
        myHead->previous = newPtr; // set head's previous to newPtr
        // update myHead to newPtr
        myHead = newPtr;
    }
    // increment size in list up
    mySize++;
    // force not sorted
    isSorted = 0;
} 

void StockAccount::printStockAccount()
{
    // if the portfolio is empty, state that you have no positions
    if (size() == 0)
        cout << "You have no positions in your portfolio\n";
    else
    {
        // check portfolio is sorted
        if(!check_if_sorted())
            bubbleSortPortfolio();

        // print out the table    
        cout << "Cash balance = " << get_CASH_BALANCE() << "\n";
        // scroll from the head to the tail and print 
        cout  << setw(15) << left << "Symbol\tQuantity\tPrice ($)\tPosition Value ($)\n";
        Position *currentPtr = myHead;
        double total_value = 0;
        while (currentPtr != 0) // stop when we reach null
        {
            double price = readFromRandomFile(currentPtr->Stock_Symbol);
            double value = price * currentPtr->Stock_Quantity;
            cout << setw(15) << left << currentPtr->Stock_Symbol;
            cout << setw(15) << left << currentPtr->Stock_Quantity;
            cout << setw(15) << left << price;
            cout << setw(15) << left << value << "\n";
            total_value = total_value + value;
            currentPtr = currentPtr->next; // set currentPtr to next, scroll right
        }
        cout << "Total Portfolio Value: $" << total_value << "\n";
    }
}

bool StockAccount::removeFromStart()
{
    // check to make sure list isn't empty
    if (size() == 0)
        return false;
    else
    {
        // if there's only one value in the list, set both the head and tail to null
        if (myHead == myTail)
            myHead = myTail = 0;
        else
        {    // set the head to the next node, and next node previous to null
            myHead = myHead->next;
            myHead->previous = NULL;
        }
        // decrement size
        mySize--;
        return true;
    }
}
 
bool StockAccount::removeFromEnd()
{
    // check to make sure list isn't empty
    if (size() == 0)
        return false;
    else
    {
        Position *tempPtr = myTail;
        if (myHead == myTail)
            myHead = myTail = 0; // set both to null
        else
        {
            // set the Position before myTail to point to null and remove tail
            myTail->previous->next = NULL;
            myTail = myTail->previous;
        }
        delete tempPtr;
        mySize--;
        return true;
    }
}

void StockAccount::removePositionFromAccount(string symbol)
{
    // check to make sure list isn't empty
    if (size() == 0)
        cout << "Error: No positions in portfolio.\n";
    else
    {
        Position *tempPtr; // use for swap
        Position *currentPtr = myHead;
        int flag = 0; // temp flag for outputting if symbol was found or not

        if (size() == 1) // special case for list size of 1
        {
            if (currentPtr->Stock_Symbol == symbol) // use string == to compare
            {
                flag = 1;
                // remove node, make list empty
                tempPtr = currentPtr;
                myHead = myTail = 0;
                delete tempPtr;
                mySize--;
            }
        } 
        else
        {
            // check to see if the head or tail contains the symbol and remove
            if (myHead->Stock_Symbol == symbol)
            {
                removeFromStart();
                flag = 1;
            }
            else if (myTail->Stock_Symbol == symbol)
            {
                removeFromEnd();
                flag = 1;
            }
            else
            {
                currentPtr = currentPtr->next;
                // search from the second position to the second to last position (the one before tail)
                while (currentPtr->next != 0)
                {
                    if (currentPtr->Stock_Symbol == symbol)
                    {
                        flag = 1;
                        tempPtr = currentPtr;
                        // make next Position's previous point currentPtr's previous
                        currentPtr->next->previous = currentPtr->previous;
                        // make currentPtr's previous point to currentPtr's next
                        currentPtr->previous->next = currentPtr->next;
                        delete tempPtr;
                        mySize--;
                    }
                    else
                        currentPtr = currentPtr->next;
                }
            }
        }
        if (flag == 0)
            cout << "Failed to find position with symbol " << symbol << "\n";  
    } 
}

bool StockAccount::check_if_sorted()
{
    if (isSorted == 1)
        return true;
    else
        return false;
}

void StockAccount::bubbleSortPortfolio()
{ // this bubble sort is based on sorting the portfolio by position's value in descending order
    if (size() == 0) // do nothing
        cout << "Error. Attempting to sort empty list.\n";
    else if (size() == 1) // special case for 1 position in portfolio
        isSorted = 1;
    else
    {
        Position *currentPtr; // start from head
        int swapped = 1; // continue to loop over list and swap until no more elements can be swapped
        while (swapped)
        {
            swapped = 0;
            currentPtr = myHead; // start from head
            for (int i = 0; i < mySize - 1; i++)
            {
                if(currentPtr->Stock_Value < currentPtr->next->Stock_Value) // sort descending by value
                {
                    swapped = 1; // will swap
                    swapPositions(currentPtr, currentPtr->next); // pairwise swap
                }
                currentPtr = currentPtr->next;
                if (currentPtr == myTail || currentPtr == NULL)
                    currentPtr = myHead;
            }
        }
        isSorted = 1;
    }
}

void StockAccount::swapPositions(Position *p1, Position *p2)
{ // pairwise swap
  // do care that p2 comes after p1

    Position *&a = p2; // currentPtr->next is p2
    Position *&b = p1; // currentPtr is p1

    p1->next = p2->next;
    p2->next = p1;
    p2->previous = p1->previous;
    p1->previous = p2;

    if (p2->previous != NULL) p2->previous->next = p2;
    if (p1->next != NULL) p1->next->previous = p1;

    // set head and tail
    if (b == myHead)
    {
        myHead = p2;
        p2->previous = 0;
    }

    if (a == myTail)
    {
        myTail = p1;
        p1->next = 0;
    }
}

bool StockAccount::checkSymbolInPortfolio(string symbol)
{
    // starting from the head to the tail, see if any position matches the given symbol
    Position *currentPtr = myHead;
    int found = 0;
    while (currentPtr != 0)
    {
        if (currentPtr->Stock_Symbol == symbol)
        {
            found = 1;
            break;
        }
        currentPtr = currentPtr->next;
    }
    if (found == 0)
        return false;
    else
        return true;
} 

void StockAccount::readFromPortfolioFile()
{
    ifstream portfolio_file;
    portfolio_file.open("portfolio.txt");
    if (!portfolio_file.good()) // if we don't have a file, complain
        cout << "Error. portfolio.txt not found.\n";
    else
    {
        string symbol = "";
        int quantity = 0;
        double value = 0; // not read from portfolio.txt, get's calculated as quantity * randomprice
        Position p = Position(symbol, quantity, value);
        while (portfolio_file >> symbol >> quantity)
            {
                p.Stock_Symbol = symbol;
                p.Stock_Quantity = quantity;
                value = readFromRandomFile(symbol) * quantity;
                p.Stock_Value = value;
                addToStart(&p);
            }
    }
    // force not sorted
    isSorted = 0;
}

double StockAccount::readFromRandomFile(string symbol)
{ /* selects from one of the two Result.txt files randomly
     Then, searches for the stock symbol within the file and 
     outputs the price. If symbol not found, complains. */
    
    string selected_file;
    srand(time(0));
    int randint = rand() % 2; // get 0 or 1
    if (randint == 1)
        selected_file = "Result_2.txt";
    else
        selected_file = "Result_1.txt";
    
    ifstream result_file;
    result_file.open(selected_file);
    if (!result_file.good()) // complain 
        cout << "Error. " << selected_file << " not found.\n";
    else
    {
        string file_symbol;
        double price;
        string date; // don't care
        while (result_file >> file_symbol >> price >> date)
            if (file_symbol == symbol)
                return price;

        cout << "Error. Stock symbol not found in file.\n";   
    }
    return -1; // flag if couldn't find symbol
}

void StockAccount::createPortfolioFile()
{ // initializes an empty portfolio file named portfolio.txt
    ifstream portfoliofile;
    portfoliofile.open("portfolio.txt");
    if(!portfoliofile.good())
    {
        ofstream file ("portfolio.txt");
        file.close();
    }
    portfoliofile.close();
}

void StockAccount::saveToPortfolioFile()
{ // saves the portfolio doubly linked list to portfolio.txt
    ofstream portfoliofile;
    portfoliofile.open("portfolio.txt");
    if(!portfoliofile.good()) // complain
        cout << "Error. Cannot find portfolio.txt\n";
    else
    {
        Position *currentPtr = myHead; // start from head, scroll, and save to file
        while (currentPtr != 0)
        {
            portfoliofile << currentPtr->Stock_Symbol << "\t" << currentPtr->Stock_Quantity << "\n";
            currentPtr = currentPtr->next;
        }
    }
    portfoliofile.close();
}

void StockAccount::createPortfolioTransactionHistory()
{ // if portfolio_transaction_history.txt does not exist, creates it
    ifstream portfolio_transaction_history;
    portfolio_transaction_history.open("portfolio_transaction_history.txt");
    if (!portfolio_transaction_history.good())
    {
        ofstream file ("portfolio_transaction_history.txt");
        file << "Event\t$Symbol\tQuantity\tPrice Per Share\tTotal Value\tDateTime\n";
        file.close();
    }
    portfolio_transaction_history.close();
}

void StockAccount::saveToPortfolioTransactionHistory(string event, string symbol, int quantity, double price, double value, string datetime)
{
    ofstream portfolio_transaction_history;
    portfolio_transaction_history.open("portfolio_transaction_history.txt", ios_base::app); // ios_base to append
    if (!portfolio_transaction_history.good()) // complain
        cout << "Error. Cannot find portfolio_transaction_history.txt\n";
    else
        portfolio_transaction_history << event << "\t" << symbol << "\t" << quantity << "\t" << price << "\t" << value << "\t" << datetime << "\n";
    portfolio_transaction_history.close();
}

void StockAccount::createPortfolioValueFile()
{ // if portfolio_value.txt does not exist, creates it
    ifstream portfolio_value;
    portfolio_value.open("portfolio_value.txt");
    if (!portfolio_value.good())
    {
        ofstream file ("portfolio_value.txt");
        file.close();
    }
    portfolio_value.close();
}

void StockAccount::savePortfolioValueFile(double value, string datetime)
{ // appends to the portfolio value file the current portfolio value. used for graphing value's history
    ofstream portfolio_value;
    portfolio_value.open("portfolio_value.txt", ios_base::app); // append to file
    if(!portfolio_value.good()) // complain
        cout << "Error. Cannot find portfolio_value.txt\n";
    else
        portfolio_value << value << "\t" << datetime << "\n";
    portfolio_value.close();
}

void StockAccount::buyShares(string symbol, int quantity)
{
    // first, check to see if position is already held in portfolio, in which case add to quantity
    Position *currentPtr = myHead;
    int found = 0;
    while (currentPtr != 0)
    {
        if (symbol == currentPtr->Stock_Symbol)
        {
            found = 1;
            currentPtr->Stock_Quantity = currentPtr->Stock_Quantity + quantity;
            currentPtr->Stock_Value = readFromRandomFile(symbol) * currentPtr->Stock_Quantity;
            break;
        }
        currentPtr = currentPtr->next;
    } // couldn't find existing position, so make a new one
    if (found == 0)
    {
        Position newPosition = Position(symbol, quantity, readFromRandomFile(symbol) * quantity);
        addToStart(&newPosition);
    }
}

void StockAccount::sellShares(string symbol, int quantity)
{
    // make sure position is held in portfolio
    Position *currentPtr = myHead;
    int found = 0;
    while (currentPtr != 0)
    {
        if (currentPtr->Stock_Symbol == symbol)
        {
            found = 1;
            break;
        }
        currentPtr = currentPtr->next;
    }
    if (found == 0)
        cout << "Error. Could not find " << symbol << " in portfolio.\n";
    else
    {
        if (quantity == currentPtr->Stock_Quantity) // going to liquidate position
            removePositionFromAccount(symbol);
        else
        {
            if (quantity > currentPtr->Stock_Quantity)
                cout << "Error. Attempting to sell more shares than held.\n";
            else
            {
                currentPtr->Stock_Quantity = currentPtr->Stock_Quantity - quantity;
                currentPtr->Stock_Value = currentPtr->Stock_Quantity * readFromRandomFile(symbol);
            }
        }
    }
}

void StockAccount::graphPortfolioValue()
{ // utilize matlab plug-in to graph portfolio value
  /*
    On call, gets the most recent portfolio value and saves to text file. Then,
    reads the portfolio value history from portfolio_value.txt
  */
    savePortfolioValueFile(getPortfolioValue(), getDate());
    Engine *engine;
    engine = engOpen(NULL);
    if (engine == NULL)
    {
        cout << "Error: Not Found.\n";
    }
    string dates[100];
    double portVals[100];
    ifstream portfolio_value;
    portfolio_value.open("portfolio_value.txt");
    if (!portfolio_value.good())
        cout << "Error. Could not find portfolio_value.txt\n";
    else
    {
        string date;
        double value;
        for (int i = 0; i < 100; i++)
        {
            portfolio_value >> value >> date;
            portVals[i] = value;
            dates[i] = date;
        }
        mxArray *A; // matrix for dates
        A = mxCreateDoubleMatrix(1, getSize(), mxREAL);
        memcpy((void *)mxGetPr(A), (void *)dates, sizeof(double) * getSize());
        engPutVariable(engine, "Dates", A);
        mxArray *B; // matrix for Portfolio Values
        B = mxCreateDoubleMatrix(1, getSize(), mxREAL);
        memcpy((void *)mxGetPr(B), (void *)portVals, sizeof(double) * getSize());
        engPutVariable(engine, "Portfolio_Value", B);
        engEvalString(engine, "plot(Dates,Portfolio_Value");

        // allow user to view graph
        system("pause");

        // remove arrays
        mxDestroyArray(A);
        mxDestroyArray(B);
        engEvalString(engine, "close;");

        // close engine out
        engClose(engine);
    }
    portfolio_value.close();
}

void StockAccount::printTransactionHistory()
{   
    cout << setw(40);
    ifstream history;
    history.open("portfolio_transaction_history.txt");
    if (!history.good())
        cout << "Error. Cannot find portfolio_transaction_history.txt\n";
    else
    {
        cout << setw(40) << "Transaction\tSymbol\tQuantity\tPurchase Price\tDate\n";
        string line;
        while (getline (history, line))
            cout << setw(40) << line << "\n";
        history.close();
    }
}

double StockAccount::getPortfolioValue()
{
    // make sure list not empty
    if (mySize == 0)
    {
        cout << "Error. No positions held in portfolio.\n";
        return 0;
    }
    else
    {
        Position *currentPtr = myHead;
        double total_value = 0;
        while (currentPtr != 0)
        {
            total_value = total_value + (currentPtr->Stock_Quantity * readFromRandomFile(currentPtr->Stock_Symbol));
            currentPtr = currentPtr->next;
        }
        return total_value;
    }
}

int StockAccount::getSize()
{
    return mySize;
}
