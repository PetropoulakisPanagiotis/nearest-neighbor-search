#pragma once
#include "../item/item.h"

/* Abstract class for sub has functions */
/* Error codes:                         */
/* Success: 0                           */
/* Invalid index: -1                    */
/* Incosistent dimensions: -2           */
class H{
    public:
        virtual ~H() = 0;

        /* Has a given item and return a value */
        virtual int hash(Item&,int&) = 0;

        /* Print statistics of hash function */
        virtual void print(void) = 0;
};

/* Sub euclidean hash function class */
class HEuclidean: public H{
    private:
        std::string id;
        Item* v; // Random item
        float t; // Random float [0,w)

    public:
        HEuclidean(std::string&,int&);
        ~HEuclidean();

        int hash(Item&,int&);
        void print(void);
};

// Petropoulakis Panagiotis
