#pragma once
#include <vector>
#include "../item/item.h"
#include "../utils/utils.h"
#include "../hashFunctions/hashFunctions.h"

/* Abstract class for sub has functions */
class neighborsProblem{
    public:
        virtual ~h() = 0;

        /* Fit the model with data - Reset existing data */
        virtual void fit(std::list<Item>& points, errorCode& status) = 0;

        /* Append new data in model */
        virtual void fitAppend(std::list<Item>& points, errorCode& status) = 0;

        /* Finds the neighbors within a given radius of an item */
        virtual void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status) = 0;
        virtual void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, errorCode& status) = 0;
        
        /* Find the K-neighbors of an item */
        virtual void kNeighbors(Item& query, int k, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status) =  0;
        virtual void kNeighbors(Item& query, int k, std::list<Item>& neighbors, errorCode& status) =  0;

        /* Accessors */
        virtual int getNumberOfPoints(errorCode& status) = 0;
        virtual int getTableSize(errorCode&,status) = 0;
 
        /* Print some statistics */
        virtual void print(void) = 0;
};

class LshEuclidean: public neighbors{
    private:

        /* Entries in hash tables */
        typedef struct entry{
            Item& point;
            int valueG; // Value of g hash function
        }entry;

        std::vector<Item> points; 
        std::vector<std::vector<std::list<entry>>> tables; // Each table is a hash table(vector of lists)
        int n; // Number of items 
        int tableSize;
        int l; // Total tables 
        int k; // Number of sub hash functions
        std::vector<hashFunctionsEuclidean>; // Each table has one hash function

    public:
        void fit(std::list<Item>& points, errorCode& status);
        void fitAppend(std::list<Item>& points, errorCode& status);

        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status);
        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, errorCode& status);
        
        void kNeighbors(Item& query, int k, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status);
        void kNeighbors(Item& query, int k, std::list<Item>& neighbors, errorCode& status);
        
        int getNumberOfPoints(errorCode& status);
        int getTableSize(errorCode&,status);

        void print(void);
};

class LshCosin: public neighbors{
    private:


    public:
        void fit(std::list<Item>& points, errorCode& status);
        void fitAppend(std::list<Item>& points, errorCode& status);

        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status);
        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, errorCode& status);
        
        void kNeighbors(Item& query, int k, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status);
        void kNeighbors(Item& query, int k, std::list<Item>& neighbors, errorCode& status);
        
        int getNumberOfPoints(errorCode& status);
        int getTableSize(errorCode&,status);

        void print(void);

};

// Petropoulakis Panagiotis
