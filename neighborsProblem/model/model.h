#pragma once
#include <vector>
#include <list>
#include "../item/item.h"
#include "../utils/utils.h"
#include "../hashFunction/hashFunction.h"

/* Abstract class for neighbors problem */
class model{
    public:
        virtual ~model() {};

        /* Fit the model with data */
        virtual void fit(std::list<Item>& points, errorCode& status) = 0;

        /* Finds the neighbors within a given radius of an item */
        virtual void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>* neighborsDistances, errorCode& status) = 0;
        
        /* Find the nearest neighbor of an item */
        virtual void nNeighbor(Item& query, Item& nNeighbor, double* neighborDistance, errorCode& status) =  0;

        /* Accessors */
        virtual int getNumberOfPoints(errorCode& status) = 0;
        virtual int getDim(errorCode& status) = 0;
        virtual unsigned size(void) = 0;

        /* Print some statistics */
        virtual void print(void) = 0;
        virtual void printHashFunctions(void) = 0;
};

// Petropoulakis Panagiotis
