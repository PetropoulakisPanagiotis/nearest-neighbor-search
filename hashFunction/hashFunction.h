#pragma once
#include <vector>
#include "../item/item.h"
#include "../utils/utils.h"

/* Forward declaration - for compare */
class hEuclidean;
class hCosin;
class hashFunctionEuclidean;
class hashFunctionCosin;

/* Abstract class for sub has functions */
class h{
    public:
        virtual ~h() = 0;

        /* Hash a given item and return a value */
        virtual int hash(Item&,errorCode&) = 0;

        /* Compare two sub has functions */
        virtual int compare(hEuclidean&,errorCode&) = 0;
        virtual int compare(hCosin&,errorCode&) = 0;

        /* Get total sub hash function */
        virtual int getCount(void) = 0;

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
        static int count;

    public:
        hEuclidean(int);
        ~hEuclidean();

        /* Overide function */
        int hash(Item&,errorCode&);

        int compare(hEuclidean&,errorCode&);
        int compare(hCosin&,errorCode&);
        
        int getCount(void);
        void print(void);
};

/* Sub cosin hash function class */
/* h(p) = 1, if r.p >= 0         */
/* h(p) = 0, if r.p < 0          */
class hCosin: public h{
    private:
        std::string id;
        Item* r; // Random item - Standard distribution
        static int count;

    public:
        hCosin(int);
        ~hCosin();

        /* Overide function */
        int hash(Item&,errorCode&);
        
        int compare(hEuclidean&,errorCode&);
        int compare(hCosin&,errorCode&);
        
        int getCount(void);
        void print(void);
};

/* Abstract class for has functions */
class hashFunction{
    public:
        virtual ~hashFunction() = 0;

        /* Hash a given item and return a value */
        virtual int hash(Item&,errorCode&) = 0;
        virtual int hashLevel2(Item&,errorCode&) = 0;

        /* Compare two has functions */
        virtual int compare(hashFunctionEuclidean&,errorCode&) = 0;
        virtual int compare(hashFunctionCosin&,errorCode&) = 0;

        /* Get total sub hash function */
        virtual int getCount(void) = 0;
        
        /* Print statistics of hash function */
        virtual void print(void) = 0;
};

/* Euclidean hash function class                                      */
/* Basic hashing: F(p) = [(r1h1(p)+...+rkhk(p)) mod M] mod table size */ 
/* Level 2 hashing: G(p) = h1(p).h2(p)...hk(p) -> Concatenation       */
class hashFunctionEuclidean: public hashFunction{
    private:
        std::string id;
        std::vector<int> R; // Random ri values - standard        
        std::vector<hEuclidean*> H; // H contains sub-hash functions        
        int k; // Number of sub hash functions
        int tableSize;
        static int count;

    public:
        hashFunctionEuclidean(int,int,int);
        ~hashFunctionEuclidean();

        /* Overide function */
        int hash(Item&,errorCode&);
        int hashLevel2(Item&,errorCode&);
 
        int compare(hashFunctionEuclidean&,errorCode&);
        int compare(hashFunctionCosin&,errorCode&);       
        
        int getCount(void);

        void print(void);
};

/* Cosin hash function class                    */
/* G(p) = h1(p).h2(p)...hk(p) -> Concatenation  */
class hashFunctionCosin: public hashFunction{
    private:
        std::string id;
        std::vector<hCosin*> H; // H contains sub-hash functions        
        int k; // Number of sub hash functions
        static int count;

    public:
        hashFunctionCosin(int,int);
        ~hashFunctionCosin();

        /* Overide function */
        int hash(Item&,errorCode&);
        int hashLevel2(Item&,errorCode&);

        int compare(hashFunctionEuclidean&,errorCode&);
        int compare(hashFunctionCosin&,errorCode&);
        
        int getCount(void);

        void print(void);
};

// Petropoulakis Panagiotis
