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

/* Default constructor */
lshCosin::lshCosin():l(5),k(4),coefficient(1/4){
    /* Set size of hash functions */
    this->hashFunctions.reserve(l);
        
    /* Set size of hash tables */
    this->tables.reserve(l);
}

lshCosin::lshCosin(int l, int k, float coefficient, errorCode& status):l(l),k(k),coefficient(coefficient){

    /* Check parameters */
    if(l < MIN_L || l > MAX_L || k < MIN_K || k > MAX_K || coefficient < MIN_C || coefficient > MAX_C){
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

lshCosin::~lshCosin(){}
/* Print statistics */
void lshCosin::print(void){

    if(this->k == -1)
        cout << "Invalid method\n";
    else{

    }
}
