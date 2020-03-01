/*
    BankAccount.h
    Author: Stephen Kenyon
    Date: 12/1/19

    Header file for BankAccount. Inherits from Account.
*/

#include <iostream>
#include <fstream>
#include "Account.h"
using namespace std;

#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

class BankAccount : public Account {

    public:
        BankAccount();
        void deposit(double);
        void withdraw(double);
        void createBankAccountHistoryFile();
        void saveToBankAccountHistoryFile(string, double, string);
        void printBankAccountTransactionHistory();
};

#endif
