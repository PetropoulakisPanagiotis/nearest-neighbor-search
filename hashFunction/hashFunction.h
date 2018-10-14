#pragma once
#include "../item/item.h"

/* Abstract class for sub has functions */
/* Error codes:                         */
/* Success: 0                           */
/* Invalid index: -1                    */
/* Incosistent dimensions: -2           */
/* Non-active hash function: -3         */
class h{
    public:
        virtual ~h() = 0;

        /* Hash a given item and return a value */
        virtual int hash(Item&,int&) = 0;

        /* Print statistics of hash function */
        virtual void print(void) = 0;
};

/* Sub euclidean hash function class */
class hEuclidean: public h{
    private:
        std::string id;
        Item* v; // Random item - Standard distribution
        float t; // Random float [0,w) - Uniform distribution

    public:
        hEuclidean(std::string&,int&);
        ~hEuclidean();

        /* Overide function */
        int hash(Item&,int&);
        void print(void);
};

/* Sub cosin hash function class */
class hCosin: public h{
    private:
        std::string id;
        Item* r; // Random item - Standard distribution

    public:
        hCosin(std::string&,int&);
        ~hCosin();

        /* Overide function */
        int hash(Item&,int&);
        void print(void);
};

// Petropoulakis Panagiotis
