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

        /* Print some statistics */
        virtual void print(void) = 0;
        virtual void printHashFunctions(void) = 0;
};

/* Neighbors problem using lsh euclidean */
class lshEuclidean: public model{
    private:
        /* Entries in hash tables */
        typedef struct entry{
            Item point;
            std::vector<int> valueG; // Value of g hash function(2 levels of hashing - compare query and point with same g)
        }entry;

        std::vector<std::vector<std::list<entry> > > tables; // Each table is a hash table(vector of lists)
        std::vector<hashFunction*> hashFunctions; // Each table has one hash function
        int tableSize;
        float coefficient; // Table size == n * coefficient, (coefficient <= 1)
        int n; // Number of items 
        int l; // Total tables, hash functions
        int k; // Number of sub hash functions
        int dim; // Dimension
        int w; // Window size
        int fitted; // Method is fitted with data
    
    public:

        lshEuclidean();
        lshEuclidean(int k, int l, errorCode& status);
        lshEuclidean(int l, int k, int w, float coefficient, errorCode& status);
        
        ~lshEuclidean();

        void fit(std::list<Item>& points, errorCode& status);

        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>* neighborsDistances, errorCode& status);
        void nNeighbor(Item& query, Item& nNeighbor, double* neighborDistance, errorCode& status);
        
        int getNumberOfPoints(errorCode& status);
        int getDim(errorCode& status);

        void print(void);
        void printHashFunctions(void);
};

/* Neighbors problem using lsh cosin */
class lshCosin: public model{
    private:
        std::vector<std::vector<std::list<Item> > > tables; // Each table is a hash table(vector of lists)
        std::vector<hashFunction*> hashFunctions; // Each table has one hash function       
        int tableSize;
        int n; // Number of items 
        int l; // Total tables 
        int k; // Number of sub hash functions
        int dim; // Dimension
        int fitted;
    
    public:

        lshCosin();
        lshCosin(int k, int l, errorCode& status);

        ~lshCosin();

        void fit(std::list<Item>& points, errorCode& status);

        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>* neighborsDistances, errorCode& status);
        void nNeighbor(Item& query, Item& nNeighbor, double* neighborDistance, errorCode& status);
        
        int getNumberOfPoints(errorCode& status);
        int getDim(errorCode& status);

        void print(void);
        void printHashFunctions(void);
};

/* Neighbors problem using hypercube euclidean */
class hypercubeEuclidean: public model{
    private:

        /* For every vertice find all neighbors and keep up to probes(sorted by hamming Distance) */
        /* With min heap                                                                          */
        typedef struct neighborVertice{
            int hammingDist; // Hamming distance with current pos
            int pos; // Position in cube 
        
            neighborVertice(int dist, int pos){
                this->hammingDist = dist;
                this->pos = pos;
            }
        }neighborVertice;

        /* Create a compare fucntion based in hamming distance*/
        struct verticesCompare{
            bool operator()(const neighborVertice& x, const neighborVertice& y) const{
                return x.hammingDist < y.hammingDist;
            }
        };

        std::vector<std::list<Item> >  cube;
        hashFunction* hashFunctions; // Hash function of cube 
        int tableSize;
        int n; // Number of items 
        int k; // Number of sub hash functions
        int dim; // Dimension
        int w; // Window size
        int m; // Max items to be searched
        int probes; // Max vertices probed
        int fitted; // Method is fitted with data
    public:

        hypercubeEuclidean();
        hypercubeEuclidean(int k, int m, int probes, errorCode& status);
        hypercubeEuclidean(int k, int m, int probes, int w, errorCode& status);
        
        ~hypercubeEuclidean();

        void fit(std::list<Item>& points, errorCode& status);

        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>* neighborsDistances, errorCode& status);
        void nNeighbor(Item& query, Item& nNeighbor, double* neighborDistance, errorCode& status);
        
        int getNumberOfPoints(errorCode& status);
        int getDim(errorCode& status);

        void print(void);
        void printHashFunctions(void);
};

/* Neighbors problem using hypercube cosin */
class hypercubeCosin: public model{
    private:

        /* For every vertice find all neighbors and keep up to probes(sorted by hamming Distance) */
        /* With min heap                                                                          */
        typedef struct neighborVertice{
            int hammingDist; // Hamming distance with current pos
            int pos; // Position in cube 
        
            neighborVertice(int dist, int pos){
                this->hammingDist = dist;
                this->pos = pos;
            }
        }neighborVertice;

        /* Create a compare fucntion based in hamming distance*/
        struct verticesCompare{
            bool operator()(const neighborVertice& x, const neighborVertice& y) const{
                return x.hammingDist < y.hammingDist;
            }
        };

        std::vector<std::list<Item> >  cube;
        hashFunction* hashFunctions; // Hash function of cube 
        int tableSize;
        int n; // Number of items 
        int k; // Number of sub hash functions
        int dim; // Dimension
        int m; // Max items to be searched
        int probes; // Max vertices probed
        int fitted; // Method is fitted with data
    public:

        hypercubeCosin();
        hypercubeCosin(int k, int m, int probes, errorCode& status);
        
        ~hypercubeCosin();

        void fit(std::list<Item>& points, errorCode& status);

        void radiusNeighbors(Item& query, int radius, std::list<Item>& neighbors, std::list<double>* neighborsDistances, errorCode& status);
        void nNeighbor(Item& query, Item& nNeighbor, double* neighborDistance, errorCode& status);
        
        int getNumberOfPoints(errorCode& status);
        int getDim(errorCode& status);

        void print(void);
        void printHashFunctions(void);
};

// Petropoulakis Panagiotis
