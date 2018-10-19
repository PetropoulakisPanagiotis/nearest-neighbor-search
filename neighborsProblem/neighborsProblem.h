#pragma once
#include <vector>
#include <list>
#include "../item/item.h"
#include "../utils/utils.h"
#include "../hashFunction/hashFunction.h"

/* Abstract class for neighbors problem */
class neighborsProblem{
    public:
        virtual ~neighborsProblem() = 0;

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
        virtual int getTableSize(errorCode& status) = 0;
        virtual int getDim(errorCode& status) = 0;

        /* Print some statistics */
        virtual void print(void) = 0;
};

/* Neighbors problem using lsh euclidean */
class lshEuclidean: public neighborsProblem{
    private:
        /* Entries in hash tables */
        typedef struct entry{
            Item& point;
            int valueG; // Value of g hash function(2 levels of hashing - compare query and point with same g)
        }entry;

        std::vector<Item*> points; 
        std::vector<std::vector<std::list<entry>>> tables; // Each table is a hash table(vector of lists)
        int tableSize;
        float coefficient; // Table size == n * coefficient, (coefficient <= 1)
        int n; // Number of items 
        int l; // Total tables, hash functions
        int k; // Number of sub hash functions
        int w; // Window size
        int dim; // Dimension
        std::vector<hashFunctionEuclidean*> hashFunctions; // Each table has one hash function

    public:

        lshEuclidean();
        lshEuclidean(int l, int k, int w, float coefficient, errorCode& status);
        
        ~lshEuclidean();

        void fit(std::list<Item>& points, errorCode& status) {};
        void fitAppend(std::list<Item>& points, errorCode& status) {};

        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status){};
        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, errorCode& status){};
        
        void kNeighbors(Item& query, int k, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status){};
        void kNeighbors(Item& query, int k, std::list<Item>& neighbors, errorCode& status){};
        
        int getNumberOfPoints(errorCode& status){};
        int getTableSize(errorCode& status){};
        int getDim(errorCode& status){};

        void print(void);
};

/* Neighbors problem using lsh cosin */
class lshCosin: public neighborsProblem{
    private:
        std::vector<Item*> points; 
        std::vector<std::vector<std::list<Item*>>> tables; // Each table is a hash table(vector of lists)
        int n; // Number of items 
        int tableSize;
        float coefficient; // Table size == n * coefficient, (coefficient <= 1)
        int l; // Total tables 
        int k; // Number of sub hash functions
        int dim; // Dimension
        std::vector<hashFunctionCosin*> hashFunctions; // Each table has one hash function       

    public:

        lshCosin();
        lshCosin(int l, int k, float coefficient, errorCode& status);

        ~lshCosin();

        void fit(std::list<Item>& points, errorCode& status){};
        void fitAppend(std::list<Item>& points, errorCode& status){};

        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status){};
        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, errorCode& status){};
        
        void kNeighbors(Item& query, int k, std::list<Item>& neighbors, std::list<double>& neighborsRadius, errorCode& status){};
        void kNeighbors(Item& query, int k, std::list<Item>& neighbors, errorCode& status){};
        
        int getNumberOfPoints(errorCode& status){};
        int getTableSize(errorCode&status){};
        int getDim(errorCode& status){};
        
        void print(void);

};

// Petropoulakis Panagiotis
