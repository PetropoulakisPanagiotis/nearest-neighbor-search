#pragma once
#include <vector>
#include <list>
#include "../model.h"
#include "../../item/item.h"
#include "../../utils/utils.h"

/* Neighbors problem using exhaustice search */
class exhaustiveSearch: public model{
    private:
        std::vector<Item> points; // All points are been kept in a single table
        int tableSize; // == n
        int n; // Number of items 
        int dim; // Dimension
        int fitted; // Method is fitted with data
    public:

        exhaustiveSearch();
        ~exhaustiveSearch();

        void fit(std::list<Item>& points, errorCode& status);

        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>* neighborsDistances, errorCode& status);
        void nNeighbor(Item& query, Item& nNeighbor, double* neighborDistance, errorCode& status);
        
        int getNumberOfPoints(errorCode& status);
        int getDim(errorCode& status);
        unsigned size(void);

        void print(void);
        void printHashFunctions(void);
};

// Petropoulakis Panagiotis
