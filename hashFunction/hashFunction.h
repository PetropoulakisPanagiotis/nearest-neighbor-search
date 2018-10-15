#pragma once
#include <vector>
#include "../item/item.h"

/* Forward declaration - for compare */
class hEuclidean;
class hCosin;

/* Abstract class for sub has functions */
class h{
    public:
        virtual ~h() = 0;

        /* Hash a given item and return a value */
        virtual int hash(Item&,int&) = 0;

        /* Compare two sub has functions */
        virtual int compare(hEuclidean&,int&) = 0;
        virtual int compare(hCosin&,int&) = 0;

        /* Print statistics of hash function */
        virtual void print(void) = 0;
};

/* Sub euclidean hash function class */
/* h(p) = floor((p . v + t) / W)     */
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
        int compare(hEuclidean&,int&);
        int compare(hCosin&,int&);
        void print(void);
};

/* Sub cosin hash function class */
/* h(p) = 1, if r.p >= 0         */
/* h(p) = 0, if r.p < 0          */
class hCosin: public h{
    private:
        std::string id;
        Item* r; // Random item - Standard distribution

    public:
        hCosin(std::string&,int&);
        ~hCosin();

        /* Overide function */
        int hash(Item&,int&);
        int compare(hEuclidean&,int&);
        int compare(hCosin&,int&);
        void print(void);
};

/* Abstract class for has functions */
class hashFunction{
    public:
        virtual ~hashFunction() = 0;

        /* Hash a given item and return a value */
        virtual int hash(Item&,int&) = 0;

        /* Print statistics of hash function */
        virtual void print(void) = 0;
};

/* Cosin hash function class                    */
/* G(p) = h1(p).h2(p)...hk(p) -> Concatenation  */
class hashFunctionCosin: public hashFunction{
    private:
        std::string id;
        std::vector<hCosin*> *H; // H contains sub-hash functions        
        int& k; // Number of sub hash functions

    public:
        hashFunctionCosin(std::string&,int&,int&);
        ~hashFunctionCosin();

        /* Overide function */
        int hash(Item&,int&);
        void print(void);
};

// Petropoulakis Panagiotis
