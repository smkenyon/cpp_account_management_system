/*
    Fuzzy_Kenyon.cpp
    Author: Stephen Kenyon
    Date:
    12/1/19

    Strategy implementation for generating additional data for the stock account.
    Given an array of existing stock data, the user can choose from one of two
    algorithms to modify what was previously tightly packed data (low variations).
    TransformTowardsRight makes the data left skewed in magnitude by applying random
    noise towards the right tail of the array. TransformTowardsLeft provides the opposite.
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Fuzzy_Kenyon.h"

using namespace std;

void TransformTowardsRight::transform(double arr[], int size)
{
    for (int i = 0; i < size; i++)
    {    
        srand(time(0));
        int randint = rand() % 100; // 0 thru 99
        double weight = randint / 100;
        arr[i] = arr[i] + (i * weight * weight); // grow as w^2
    }
}

void TransformTowardsLeft::transform(double arr[], int size)
{
    for (int i = size -1 ; i >= 0; i--)
    {
        srand(time(0));
        int randint = rand() % 100; // 0 thru 99
        double weight = randint / 100;
        arr[i] = arr[i] + (i * weight * weight);
    }
}