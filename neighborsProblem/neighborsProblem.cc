#include <iostream>
#include <vector>
#include <list>
#include "../hashFunction/hashFunction.h"
#include "../item/item.h"
#include "../utils/utils.h"
#include "neighborsProblem.h"

using namespace std;

/////////////////////////////////////////////////////////
/* Implementation of abstract neighbors Problem        */
/* Note: No default dehaviors - behave like inrerface  */
/////////////////////////////////////////////////////////

neighborsProblem::~neighborsProblem(){}

///////////////////////////////////////////
/* Implementation of lsh euclidean class */
///////////////////////////////////////////

/* Default constructor */
lshEuclidean::lshEuclidean():l(5),k(4),w(200),coefficient(1/4){
    /* Set size of hash functions */
    this->hashFunctions.reserve(l);
        
    /* Set size of hash tables */
    this->tables.reserve(l);
}

lshEuclidean::lshEuclidean(int l, int k, int w, float coefficient, errorCode& status):l(l),k(k),w(w),coefficient(coefficient){

    /* Check parameters */
    if(l < MIN_L || l > MAX_L || k < MIN_K || k > MAX_K || w < MIN_W || w > MAX_W || coefficient < MIN_C || coefficient > MAX_C){
        status = INVALID_PARAMETERS;
        this->k = -1;
    }
    else{
      
        /* Set size of hash functions */
        this->hashFunctions.reserve(l);
        
        /* Set size of hash tables */
        this->tables.reserve(l);
    }
}

lshEuclidean::~lshEuclidean(){}

/* Fix hash table, members of lshEuclidean and add given points in the hash tables */
void lshEuclidean::fit(list<Item>& points, errorCode& status){
    int i, j, k;
    int flag = 0; // Invalid points
    int pos; // Pos(line) in current hash table
    entry newEntry; // In hash table
    list<Item>::iterator iterPoints = points.begin(); // Iterate through points
    list<entry>::iterator iterEntries;  // Iterate through entries
    
    status = SUCCESS;

    /* Check method */
    if(this->k == -1){
        status = INVALID_METHOD;
        return;
    }

    /* Check parameters */
    if(points.size() > MAX_DIM || points.size() <= 0){
        status = INVALID_DIM;
        return;
    }

    /* Set members */
    this->n = points.size();
    if(this->n < MIN_POINTS || this->n > MAX_POINTS){
        status = INVALID_POINTS;
        return;
    }

    /* Set table size and all hash tables */
    this->tableSize = (int)(this->n * this->coefficient);
    for(i = 0; i < this->tableSize; i++)
        this->tables[i].reserve(this->tableSize);

    /* Set dimension */
    this->dim = iterPoints->getDim();
    if(this->dim <= 0){
        status = INVALID_DIM;
        return;
    }
    
    ////////////////////////
    /* Set hash functions */
    ////////////////////////
    hashFunctionEuclidean* newFunc; // Function that will be inserted in hash functions table
    
    for(i = 0; i < this->l; i++){

        newFunc = new hashFunctionEuclidean(this->dim, this->k, this->w, this->tableSize);

        /* Truncate same hash functions */
        for(j = 0; j < i; j++){
            this->hashFunctions[i]->print();
            newFunc->print();

            if(this->hashFunctions[i]->compare(*newFunc,status) == 0){
                delete newFunc;
                break;
            }
        } // End for

        if(i == j)
            this->hashFunctions[i] = newFunc; // Add hash function
        else
            i -= 1;
    } // End for - Hash functions

    /////////////////////
    /* Set hash tables */
    /////////////////////

    for(i = 0; i < this->l; i++){
        
        /* Scan given points */
        for(iterPoints = points.begin(); iterPoints != points.end(); iterPoints++){
            /* Check consistency of dim */
            if(this->dim != iterPoints->getDim()){
                status = INVALID_DIM;
                flag = 1;
                break;
            }
            
            /* Find position in hash table */
            pos = this->hashFunctions[i]->hash(*iterPoints,status);
            if(pos < 0 || pos >= tableSize){
                status = INVALID_HASH_FUNCTION;
                flag = 1;
                break;
            }
            
            /* Set new entry */
            newEntry.point = new Item(*iterPoints);
            newEntry.valueG = this->hashFunctions[i]->hashLevel2(*iterPoints,status);

            /* Add item */
            this->tables[i][pos].push_back(newEntry);
        } // End for - Points

        if(flag == 1)
            break;
    } // End for - Hash tables

    ///////////////////////////////////////////
    /* Error occured - Delete all structures */
    //////////////////////////////////////////
    if(flag == 0){
        
        /* Scan tables */
        for(j = 0; j < i; j++){
            
            /* Scan lines */
            for(k = 0; k < this->tableSize; k++){
    
                /* Scan list */
                for(iterEntries = this->tables[j][k].begin(); iterEntries != this->tables[j][k].end(); iterEntries++){
                

                    iterEntries->point->print();
                } // End for - Iterate through list 
            } // End for - Line of current table
        } // End for - Hash tables

    } // End if error occured 
}

/* Print statistics */
void lshEuclidean::print(void){

    if(this->k == -1)
        cout << "Invalid method\n";
    else{

    }
}

///////////////////////////////////////
/* Implementation of lsh cosin class */
///////////////////////////////////////

