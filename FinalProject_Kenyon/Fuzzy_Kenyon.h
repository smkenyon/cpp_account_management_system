/*
    Fuzzy_Kenyon.h
    Author: Stephen Kenyon
    Date: 12/1/19

    Strategy implementation for generating additional
    data for the stock account. Given an array
    of existing stock data, the user can choose an 
    algorithm to modify the data in a deterministic way.
*/

#include <iostream>
using namespace std;

#ifndef FUZZY_KENYON_H
#define FUZZY_KENYON_H

class TransformImpl
{
    public:
        virtual void transform(double [], int) = 0;
};

class TransformTowardsRight : public TransformImpl
{
    public:
        void transform(double [], int);
};

class TransformTowardsLeft : public TransformImpl
{
    public:
        void transform(double [], int);
};
#endif