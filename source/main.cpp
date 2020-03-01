/*
    main.cpp
    Author: Stephen Kenyon
    Date: 12/1/19

Main instance of Portolio Program. See Word Documentation for additional details.
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include "Position.h"
#include "StockAccount.h"
#include "Account.h"
#include "BankAccount.h"
#include "Fuzzy_Kenyon.h"
#include "FileReadAdapter.h"

using namespace std;

int main ()
{
    cout << fixed;
    cout << setprecision(2); // set globally to 2 decimals
    // on program load, create Bank Account and Stock Account
    BankAccount bank_account = BankAccount();
    StockAccount stock_account = StockAccount();
    
    int flag = 0; // flag used to control exiting the program
    int upper_case_switch; // switch case selector for top level

    // Title
    cout << "Welcome to the Account Management System.\n";

    while (true)
    {
        // Top Menu
        cout << "Please select an account to access:\n";
        cout << "1. Stock Portfolio Account\n";
        cout << "2. Bank Account\n";
        cout << "3. Exit\n";
        cin >> upper_case_switch;

        switch (upper_case_switch)
        {
        case (1):
            {
                // Stock Portfolio Account Menu
                int stock_case_switch;
                cout << "Please select an option:\n";
                cout << "1. Display the price for a stock symbol\n";
                cout << "2. Display the current portfolio\n";
                cout << "3. Buy shares\n";
                cout << "4. Sell shares\n";
                cout << "5. View a graph for the portfolio value\n";
                cout << "6. View transaction history\n";
                cout << "7. Experimental features\n";
                cout << "8. Return to previous menu\n";
                cin >> stock_case_switch;

                switch(stock_case_switch)
                {
                    case(1):
                        {  // stock price lookup
                            cout << "Enter the symbol that you wish to lookup. All uppercase, please.\n";
                            string symbol;
                            cin >> symbol;
                            double price = stock_account.readFromRandomFile(symbol);
                            cout << "The current price of $" << symbol << " is: " << "$" << price << endl;
                            break;
                        }
                    case(2):
                        {  // print stock account
                            stock_account.printStockAccount();
                            break;
                        }
                    case(3):
                        {   // buy shares
                            if (stock_account.get_CASH_BALANCE() > 0)
                            {
                                cout << "Enter the symbol to lookup stock quote.\n";
                                string symbol;
                                cin >> symbol;
                                cout << "The current price is " << stock_account.readFromRandomFile(symbol) << "\n";
                                cout << "Your current cash balance is: " << bank_account.get_CASH_BALANCE() << "\n";
                                cout << "Enter a limit price for your order: ";
                                double limit_price;
                                cin >> limit_price;
                                cout << "Enter share quantity: ";
                                int quantity;
                                cin >> quantity;
                                while ((limit_price * quantity) > stock_account.get_CASH_BALANCE())
                                {
                                    cout << "Error. Limit Price * Quantity is greater than free cash balance.\n";
                                    cout << "Reduce quantity of shares or limit price.\n";
                                    cout << "Limit Price: ";
                                    cin >> limit_price;
                                    cout << "Share Quantity: ";
                                    cin >> quantity;
                                }
                                cout << "Purchase " << symbol << " " << quantity << " shares at $" << limit_price << " for a total of $" << quantity * limit_price << endl;
                                cout << "Confirm? 1 if yes, 2 if no.\n"; // lazy confirmation
                                int confirmation;
                                cin >> confirmation;
                                if (confirmation == 1)
                                {
                                    double current_price = stock_account.readFromRandomFile(symbol);
                                    if (limit_price >= current_price)
                                    {
                                        stock_account.buyShares(symbol, quantity);
                                        stock_account.bubbleSortPortfolio();
                                        stock_account.saveToPortfolioTransactionHistory("stock purchase", symbol, quantity, current_price, quantity * current_price, stock_account.getDate());
                                        bank_account.withdraw(current_price * quantity);
                                        cout << "Successfully purchased " << quantity << " shares of stock at a price of " << current_price << "\n";
                                        cout << "Your new free-cash balance is: " << stock_account.get_CASH_BALANCE() << "\n\n";
                                    }
                                    else
                                        cout << "Failed to purchase stock. Price has moved beyond limit price.\n";
                                }
                            }
                            else
                            {
                                cout << "You don't have a sufficient cash balance to make a purchase.\n";
                                cout << "Consider selling other positions or depositing more money to your bank account.\n";
                            }
                            break;
                        }
                    case(4):
                        { // sell shares
                            if (stock_account.size() == 0)
                                cout << "You don't have any positions in your portfolio.\n";
                            else
                            {
                                cout << "Enter a stock symbol for a position you wish to sell from.\n";
                                string symbol;
                                cin >> symbol;
                                if(!stock_account.checkSymbolInPortfolio(symbol))
                                    cout << "Error. Could not find " << symbol << " in your portfolio.\n";
                                else
                                {
                                    cout << "Enter a price you wish to sell at.\n";
                                    double price;
                                    cin >> price;
                                    cout << "Enter the number of shares you wish to sell.\n";
                                    int shares;
                                    cin >> shares;
                                    cout << "Sell $" << symbol << " " << shares << " shares at a limit price of $" << price << " for a total of $" << shares * price << endl;
                                    cout << "1 to confirm.\n";
                                    int confirmation;
                                    cin >> confirmation;
                                    if (confirmation == 1)
                                    {
                                        double current_price = stock_account.readFromRandomFile(symbol);
                                        if (price <= current_price)
                                        {
                                            stock_account.sellShares(symbol, shares);
                                            stock_account.bubbleSortPortfolio();
                                            stock_account.saveToPortfolioTransactionHistory("stock sale", symbol, shares, current_price, shares * current_price, stock_account.getDate());
                                            bank_account.saveToBankAccountHistoryFile("deposit - stock transaction", current_price * shares, bank_account.getDate());
                                            cout << "Successfully sold " << shares << " shares of stock at a price of " << current_price << "\n";
                                            cout << "Your new free-cash balance is: " << stock_account.get_CASH_BALANCE() << "\n\n";
                                        }
                                        else
                                            cout << "Failed to sell stock. Price has moved below limit price.\n";
                                    }
                                }
                            }
                            break;
                        }
                    case(5):
                        { // matlab plot
                            stock_account.graphPortfolioValue();
                            break;
                        }
                    case(6):
                        { // view stock transaction history
                            cout << "Displaying Stock Transaction History.\n\n";
                            stock_account.printTransactionHistory();
                            break;
                        }
                    case(7):
                        { // experimental features
                            cout << "Experimental feature for additional random price generation.\n";
                            cout << "Gets a set of fixed random prices based on MSFT, and applies a linear transformation to the data.\n";
                            cout << "To begin, select if you prefer a right-skewed or left-skewed distribution. (1 for right-skewed, 2 for left-skewed)\n";
                            int dist;
                            cin >> dist;
                            while (dist < 1 || dist > 2)
                            {
                                cout << "Error. Please select 1 or 2.\n";
                                cin >> dist;
                            }
                            if (dist == 1)
                            {
                                FileReadAdapter adp;
                                adp.readFile(stock_account);
                                TransformTowardsRight tR; 
                                // can purchase shares of MSFT using new random value
                                cout << "Using these new values, would you like to pick a random value for MSFT and purchase a single share? 1 if yes, 2 if no\n";
                                int pur;
                                cin >> pur;
                                while (pur < 1 || pur > 2)
                                {
                                    cout << "Error. Please select 1 or 2.\n";
                                    cin >> pur;
                                }
                                if (pur == 1)
                                {
                                    srand(time(0));
                                    int randint = rand() % stock_account.getSize();
                                    double current_price = adp.array[randint];
                                    double *a = adp.array;
                                    tR.transform(a, stock_account.getSize());
                                    stock_account.buyShares("MSFT", 1);
                                    stock_account.bubbleSortPortfolio();
                                    stock_account.saveToPortfolioTransactionHistory("stock purchase", "MSFT", 1, current_price, 1 * current_price, stock_account.getDate());
                                    bank_account.withdraw(current_price * 1);
                                    cout << "Successfully purchased one share of MSFT at a price of " << current_price << "\n";
                                    cout << "Your new free-cash balance is: " << stock_account.get_CASH_BALANCE() << "\n\n";
                                }
                            }
                            else
                            {
                                FileReadAdapter adp;
                                adp.readFile(stock_account);
                                TransformTowardsLeft tL;
                                // can purchase shares of MSFT using new random value
                                cout << "Using these new values, would you like to pick a random value for MSFT and purchase a single share? 1 if yes, 2 if no\n";
                                int pur;
                                cin >> pur;
                                while (pur < 1 || pur > 2)
                                {
                                    cout << "Error. Please select 1 or 2.\n";
                                    cin >> pur;
                                }
                                if (pur == 1)
                                {
                                    srand(time(0));
                                    int randint = rand() % stock_account.getSize();
                                    double current_price = adp.array[randint];
                                    double *a = adp.array;
                                    tL.transform(a, stock_account.getSize());
                                    stock_account.buyShares("MSFT", 1);
                                    stock_account.bubbleSortPortfolio();
                                    stock_account.saveToPortfolioTransactionHistory("stock purchase", "MSFT", 1, current_price, 1 * current_price, stock_account.getDate());
                                    bank_account.withdraw(current_price * 1);
                                    cout << "Successfully purchased one share of MSFT at a price of " << current_price << "\n";
                                    cout << "Your new free-cash balance is: " << stock_account.get_CASH_BALANCE() << "\n\n";
                                }
                            }
                            
                            break;
                        }
                    case(8):
                        {   // return to the main menu
                            break;
                        }
                    default:
                        {
                            cout << "Error. Please enter a number 1 through 7.\n";
                            break;
                        }
                }
                break;
            }
        case (2):
            {
                // Bank Account Menu
                cout << "Please select an option:\n";
                cout << "1. View account balance\n";
                cout << "2. Deposit money\n";
                cout << "3. Withdraw money\n";
                cout << "4. Print out history\n";
                cout << "5. Return to the previous menu\n";
                int bank_account_menu;
                cin >> bank_account_menu;

                switch (bank_account_menu)
                {
                    case(1):
                        {   // account balance (free cash)
                            cout << "Your current account balance (free cash): ";
                            cout << bank_account.get_CASH_BALANCE();
                            cout << "\n\n";
                            break;
                        }
                    case(2):
                        {  // Deposit Money
                            cout << "Enter the amount you wish to deposit.\n";
                            double amount;
                            cin >> amount;
                            while (amount <= 0)
                            {
                                cout << "Error. Enter a non-negative value to deposit.\n";
                                cin >> amount;
                            }
                            bank_account.deposit(amount);
                            cout << "Your cash balance is now: $" << bank_account.get_CASH_BALANCE() << "\n\n";
                            break;
                        }
                    case(3):
                        {  // Withdraw Money
                            cout << "Enter the amount you wish to withdraw.\n";
                            double amount;
                            cin >> amount;
                            while (amount <= 0)
                            {
                                cout << "Error. Enter a non-negative value to withdraw.\n";
                                cin >> amount;
                            }
                            bank_account.withdraw(amount);
                            cout << "Your cash balance is now: $" << bank_account.get_CASH_BALANCE() << "\n\n";
                            break;
                        }
                    case(4):
                        {  // Print transaction history
                            cout << "Printing Bank Account Transaction History:\n";
                            bank_account.printBankAccountTransactionHistory();
                            cout << "\n\n";
                            break;
                        }
                    case(5):
                        {   // previous menu
                            break;
                        }
                    default:
                        {
                            cout << "Error. Please enter a number 1 through 5.\n";
                            break;
                        }
                }
                break;
            }
        case (3):
            {
                flag = 1;
                cout << "Exiting the program." << endl;
                // take care of file saving here
                bank_account.saveAccountFile();
                stock_account.savePortfolioValueFile(stock_account.getPortfolioValue(),stock_account.getDate());
                stock_account.saveToPortfolioFile();
                break;
            }
        default: 
            {
                cout << "Error. Please enter a number, 1 through 3.\n";
                break;
            }
        }
        if (flag == 1) // exit the program
            break;
    }
    return 0;
}