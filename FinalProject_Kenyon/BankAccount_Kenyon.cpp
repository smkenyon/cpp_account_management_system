/*
    BankAccount.cpp
    Author: Stephen Kenyon
    Date: 12/1/19

    Member function definitions for BankAccount.h
*/

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include "BankAccount_Kenyon.h"

using namespace std;



BankAccount::BankAccount() : Account()
{
    // load account history file and if not found, create one
    ifstream account_history;
    account_history.open("bank_transaction_history.txt");
    if (!account_history.good())
        createBankAccountHistoryFile();
}

void BankAccount::deposit(double amount)
{
    if (amount >= 0) 
    {
        string date = getDate();
        set_CASH_BALANCE(get_CASH_BALANCE() + amount);
        saveToBankAccountHistoryFile("Deposit", amount, date);
    }
    else
        cout << "Error. Cannot deposit negative money!\n";
}

void BankAccount::withdraw(double amount)
{
    if (amount >= 0)
        if ((get_CASH_BALANCE() - amount) >= 0)
        {  
            string date = getDate();
            set_CASH_BALANCE(get_CASH_BALANCE() - amount);
            saveToBankAccountHistoryFile("Withdrawal", amount, date);
        }
        else
            cout << "Error. Cannot overdraft!\n";
    else
        cout << "Error. Cannot withdraw negative money!\n";
}

void BankAccount::createBankAccountHistoryFile()
{   
    ifstream accountfile;
    accountfile.open("bank_transaction_history.txt"); // current name for bank account history file
    if (!accountfile.good())
    {
        ofstream file ("bank_transaction_history.txt");
        file << "Event\tAmount\tDate\tBalance\n";
        file.close();
    }
    accountfile.close();
}

void BankAccount::saveToBankAccountHistoryFile(string event, double amount, string _date)
{
    ofstream accountfile;
    accountfile.open("bank_transaction_history.txt", ios_base::app);  // ios_base to append to file
    if (!accountfile.good())
        cout << "Error. Cannot find bank_transaction_history.txt\n";
    else
        accountfile << event << "\t" << amount << "\t" << _date << "\t" << get_CASH_BALANCE() << "\n";
    accountfile.close();
}

void BankAccount::printBankAccountTransactionHistory()
{
    cout << setw(40);
    ifstream accountfile;
    accountfile.open("bank_transaction_history.txt");
    if (!accountfile.good())
        cout << "Error. Cannot find bank_transaction_history.txt\n";
    else
    {
        string line;
        while (getline (accountfile, line))
        {
            cout << setw(40) << left << line << "\n";
        }
        accountfile.close();
    }
}
