#pragma once
#include <vector>
#include <unordered_map>
#include <random>
#include "../item/item.h"
#include "../utils/utils.h"

/* Forward declaration - for compare */
class hEuclidean;
class hCosin;
class hashFunctionEuclidean;
class hashFunctionCosin;
class hashFunctionEuclideanHypercube;

/* Abstract class for sub has functions */
class h{
    public:
        virtual ~h() = 0;

        /* Hash a given item and return a value */
        virtual int hash(Item& p, errorCode& status) = 0;

        /* Compare two sub has functions */
        virtual int compare(hEuclidean& x, errorCode& status) = 0;
        virtual int compare(hCosin& x, errorCode& status) = 0;

        /* Get size of hash functions */
        virtual unsigned size(void) = 0;

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
        int w; // Window size
        static int count;

    public:
        hEuclidean(int dim, int w);
        ~hEuclidean();

        /* Overide functions */
        int hash(Item& p, errorCode& status);

        int compare(hEuclidean& x, errorCode& status);
        int compare(hCosin& x, errorCode& status);
        
        unsigned size(void);
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

        /* Overide functions */
        int hash(Item& p, errorCode& status);
        
        int compare(hEuclidean& x, errorCode& status);
        int compare(hCosin& x, errorCode& status);
        
        unsigned size(void);
        int getCount(void);
        void print(void);
};

/* Abstract class for has functions */
class hashFunction{
    public:
        virtual ~hashFunction() = 0;

        /* Hash a given item and return a value */
        virtual int hash(Item& p, errorCode& status) = 0;
        virtual int hashSubFunction(Item&p, int index, errorCode& status) = 0;


        /* Compare two has functions */
        virtual int compare(hashFunctionEuclidean& x, errorCode& status) = 0;
        virtual int compare(hashFunctionCosin& x, errorCode& status) = 0;
        virtual int compare(hashFunctionEuclideanHypercube& x, errorCode& status) = 0;
       
        /* Get size */
        virtual unsigned size(void) = 0;
        
        /* Get total sub hash function */
        virtual int getCount(void) = 0;
        
        /* Print statistics of hash function */
        virtual void print(void) = 0;
};

/* Euclidean hash function class                                */
/* Basic hashing: F(p) = [(r1h1(p)+...+rkhk(p))] mod table size */ 
/* Level 2 hashing: G(p) = {h1(p),h2(p),...,hk(p)}              */
class hashFunctionEuclidean: public hashFunction{
    private:
        std::string id;
        std::vector<int> R; // Random ri values - standard        
        std::vector<hEuclidean*> H; // H contains sub-hash functions        
        int k; // Number of sub hash functions
        int w; // Window size
        int tableSize;
        static int count;

    public:
        hashFunctionEuclidean(int dim, int k, int w, int tableSize);
        ~hashFunctionEuclidean();

        /* Overide functions */
        int hash(Item& p, errorCode& status);
        int hashSubFunction(Item&p, int index, errorCode& status);
        
        int compare(hashFunctionEuclidean& x, errorCode& status);
        int compare(hashFunctionCosin& x, errorCode& status);       
        int compare(hashFunctionEuclideanHypercube& x, errorCode& status);
        
        unsigned size(void);
        int getCount(void);
        void print(void);
};

/* Cosin hash function class                    */
/* G(p) = h1(p).h2(p)...hk(p) -> Concatenation  */
class hashFunctionCosin: public hashFunction{
    private:
        std::string id;
        std::vector<hCosin*> H; // H contains sub hash functions        
        int k; // Number of sub hash functions
        static int count;

    public:
        hashFunctionCosin(int dim, int k);
        ~hashFunctionCosin();

        /* Overide functions */
        int hash(Item& p, errorCode& status);
        int hashSubFunction(Item&p, int index, errorCode& status);

        int compare(hashFunctionEuclidean& x, errorCode& status);
        int compare(hashFunctionCosin& x, errorCode& status);
        int compare(hashFunctionEuclideanHypercube& x, errorCode& status);
        
        unsigned size(void);
        int getCount(void);
        void print(void);
};

/* Euclidean hash function for hypercube class              */
/* G(p) = f1(h1(p)).f2(h2(p))...fk(hk(p)) -> Concatenation  */
class hashFunctionEuclideanHypercube: public hashFunction{
    private:
        std::string id;
        std::vector<hEuclidean*> H; // H contains sub hash functions        
        std::vector<std::unordered_map<int, int>  > hMaps; // Keep in map f unique values
        int k; // Number of sub hash functions
        int w; // Window size
        std::vector<std::uniform_int_distribution<int> > dist; // DIstributions for fi 
        std::default_random_engine generator; 
        static int count;

    public:
        hashFunctionEuclideanHypercube(int dim, int k, int w);
        ~hashFunctionEuclideanHypercube();

        /* Overide functions */
        int hash(Item& p, errorCode& status);
        int hashSubFunction(Item&p, int index, errorCode& status);

        int compare(hashFunctionEuclidean& x, errorCode& status);
        int compare(hashFunctionCosin& x, errorCode& status);
        int compare(hashFunctionEuclideanHypercube& x, errorCode& status);
        
        unsigned size(void);
        int getCount(void);
        void print(void);
};

// Petropoulakis Panagiotis
