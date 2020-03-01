/*
    FileReadAdapter.h
    Author: Stephen Kenyon
    Date: 12/1/19

    Adapter pattern to transform the output of double from readRandomFile in
    StockAccount to an array of doubles.
*/

#include <iostream>
#include <vector>
#include "StockAccount_Kenyon.h"
using namespace std;

#ifndef FILEREADAPTER_KENYON_H
#define FILEREADAPTER_KENYON_H

class FileReadAdapter {

    public:
        void readFile(StockAccount);
        double array[100];
};

#endif