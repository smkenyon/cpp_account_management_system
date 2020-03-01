/*
    FileReadAdapter.cpp
    Author: Stephen Kenyon
    Date: 12/1/19

    Member function definitions for FileReadAdapter. Converts a set of file reads
    from a fixed file, in this case hard-coded to "MSFT" to an array. This is useful,
    because when we combine this we the algos from Fuzzy_Kenyon, we can generate
    additional random data with greater spread/variance. The adapter is needed
    because Fuzzy_Kenyon.cpp requires an array to work on, and there are no available
    arrays in the source class StockAccount.
*/

#include <iostream>
#include "FileReadAdapter.h"
#include "StockAccount.h"

using namespace std;

void FileReadAdapter::readFile(StockAccount account)
{
    array[account.getSize()];
    for (int i = 0; i < account.getSize(); i++)
    {
        account.readFromRandomFile("MSFT");
    }
}