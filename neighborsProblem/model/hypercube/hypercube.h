#pragma once
#include <vector>
#include <list>
#include "../model.h"
#include "../../item/item.h"
#include "../../utils/utils.h"
#include "../../hashFunction/hashFunction.h"

/* Neighbors problem using hypercube euclidean */
class hypercubeEuclidean: public model{
    private:

        /* For every vertice find all neighbors and scann up to probes vertices(sorted by hamming Distance) */
        /* With min heap                                                                                    */
        typedef struct neighborVertice{
            int hammingDist; // Hamming distance with current pos
            int pos; // Position in cube 
        
            neighborVertice(int dist, int pos){
                this->hammingDist = dist;
                this->pos = pos;
            }
        }neighborVertice;

        /* Create a compare class based in hamming distance*/
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
        unsigned size();

        void print(void);
        void printHashFunctions(void);
};

/* Neighbors problem using hypercube cosine */
class hypercubeCosine: public model{
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

        hypercubeCosine();
        hypercubeCosine(int k, int m, int probes, errorCode& status);
        
        ~hypercubeCosine();

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
