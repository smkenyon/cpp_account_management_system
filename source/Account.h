/*
    Account.h
    Author: Stephen Kenyon
    Date: 12/1/19

    Header file for abstract base class "Account".
*/

#include <iostream>
#include <string>
using namespace std;

#ifndef ACCOUNT_H
#define ACCOUNT_H

class Account {

    public:
        Account(); // default constructor will set CASh_BALANCE to 10,000
        Account(double); 
        double get_CASH_BALANCE();
        void set_CASH_BALANCE(double);

        void createAccountFile();
        void readAccountFile();
        void saveAccountFile();
        string getDate();

    private:
        static double CASH_BALANCE;
    

        
        
};

#endif
