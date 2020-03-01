/*
    FileReadAdapter.h
    Author: Stephen Kenyon
    Date: 12/1/19

    Adapter pattern to transform the output of double from readRandomFile in
    StockAccount to an array of doubles.
*/

#include <iostream>
#include <vector>
#include "StockAccount.h"
using namespace std;

#ifndef FILEREADAPTER_H
#define FILEREADAPTER_H

class FileReadAdapter {

    public:
        void readFile(StockAccount);
        double array[100];
};

#endif