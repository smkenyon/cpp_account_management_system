/*
    Account.cpp
    Author: Stephen Kenyon
    Date: 12/1/19

    Methods for Account class. Class header files located in Account.h
*/

#include <iostream>
#include <fstream>
#include <string>
#include "Account_Kenyon.h"
using namespace std;

double Account::CASH_BALANCE = 0;

// default constructor sets CASH_BALANCE to 10000
Account::Account()
{
    ifstream accountfile;
    accountfile.open("account_value.txt");
    if (!accountfile.good())
    {
        createAccountFile();
        set_CASH_BALANCE(10000);
        saveAccountFile();
    }
    else
        readAccountFile();
}

// regular constructor allows for different integer values for cash balance
Account::Account(double balance)
{
    set_CASH_BALANCE(balance);
}

double Account::get_CASH_BALANCE()
{
    return CASH_BALANCE;
    return 0;
}

void Account::set_CASH_BALANCE(double balance)
{   // we do not allow for trading on margin in this program, or for writing derivatives.
    if (balance >= 0)
        CASH_BALANCE = balance;
    else    // thankfully, we never allow for overdraft, nor do we charge for it
        cout << "Error, cannot set CASH_BALANCE to be negative.\n";
    CASH_BALANCE = balance;
}

void Account::createAccountFile()
{
    ifstream accountfile;
    accountfile.open("account_value.txt");
    if(!accountfile.good())
    {
        ofstream file ("account_value.txt");
        file.close();
    }
    accountfile.close();
}

void Account::readAccountFile()
{
    ifstream accountfile;
    accountfile.open("account_value.txt");
    if(!accountfile.good())
        cout << "Error. account_value.txt not found.\n";
    else
    {
        string value;
        getline(accountfile, value);
        set_CASH_BALANCE(stod(value)); // converts string to double for setting cash balance
    }
    accountfile.close();
}

void Account::saveAccountFile()
{
    ofstream accountfile;
    accountfile.open("account_value.txt");
    if(!accountfile.good())
        cout << "Error. account_value.txt not found.\n";
    else
        accountfile << get_CASH_BALANCE();
    accountfile.close();
}

string Account::getDate()
{
    string date; // get the current date from the time struct
    time_t current_time;
    struct tm* timestruct;
    time(&current_time);
    timestruct = localtime(&current_time);
    int day = timestruct->tm_mday;
    int year = timestruct->tm_year + 1900;
    int month = timestruct->tm_mon;
    date = to_string(month + 1) + "/" + to_string(day) + "/" + to_string(year); // c++11
    return date;
}