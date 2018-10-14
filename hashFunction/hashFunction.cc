#include <iostream>
#include <vector>
#include <math.h>
#include "hashFunction.h"
#include "../item/item.h"
#include "../utils/utils.h"
#include "../myLimits/myLimits.h"

using namespace std;

/////////////////////////////////////////////////////////
/* Implementation of abstract sub-hash functions class */
/* Note: No default dehaviors - behave like inrerface  */
/////////////////////////////////////////////////////////

h::~h(){}

/////////////////////////////////////////////////////////
/* Implementation of sub euclidean hash function class */
/////////////////////////////////////////////////////////

hEuclidean::hEuclidean(string& id, int& dim){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM){
        this->v = NULL;
    }
    else{ 
        /* Temporary vector for item v */
        vector<double> components(dim);
        int i = 0, status;

        /* Fix id */
        this->id = id;

        /* Pick a random t - uniform distribution */
        this->t = getRandom(0);

        /* Fix item - Pick random float in standard distribution */
        for(i = 0; i < dim; i++)
            components[i] = getRandom(1);

        /* Create item v */
        this->v = new Item(id,components,status);
        if(status != 0){
            delete this->v;
            this->v = NULL;
        } 
    }
}

/* Destructor */
hEuclidean::~hEuclidean(){
    if(v != NULL)
        delete v;
}

/* Calculate hash value of given p item */
/* h(p) = floor((p . v + t) / W)        */
int hEuclidean::hash(Item& p, int& status){
    float innerProduct;
    int result;

    status = 0;
    if(this->v == NULL){
        status = -3;
        return -1;
    }

    innerProduct = (float)p.innerProduct(*(this->v),status);
    if(status != 0)
        return -1;

    result = floor((innerProduct + this->t) / W);

    /* Truncate negative values */
    result = abs(result);

    return result;
}

/* Print statistics of sub hash function */
void hEuclidean::print(void){

    if(this->v == NULL)
        cout << "Invalid euclidean function\n";
    else{
        cout << "Euclidean(h) id: " << this->id << "\n";
        cout << "Value of t: " << this->t  << "\n"; 
        cout << "Statistics of v: \n"; 
        this->v->print();
    }
}

/////////////////////////////////////////////////////
/* Implementation of sub cosin hash function class */
/////////////////////////////////////////////////////

hCosin::hCosin(string& id, int& dim){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM){
        this->r = NULL;
    }
    else{ 
        /* Temporary vector for item r */
        vector<double> components(dim);
        int i = 0, status;

        /* Fix id */
        this->id = id;

        /* Fix item - Pick random float in standard distribution */
        for(i = 0; i < dim; i++)
            components[i] = getRandom(1);

        this->r = new Item(id,components,status);
        if(status != 0){
            delete this->r;
            this->r = NULL;
        } 
    }
}

/* Destructor */
hCosin::~hCosin(){
    if(r != NULL)
        delete r;
}

/* Calculate hash value of given p item */
/* h(p) = 2, if r.p >= 0                */
/* h(p) = 0, if r.p < 0                 */
int hCosin::hash(Item& p, int& status){
    float innerProduct;
    int result;

    status = 0;
    if(this->r == NULL){
        status = -3;
        return -1;
    }

    innerProduct = (float)p.innerProduct(*(this->r),status);
    if(status != 0)
        return -1;

    if(innerProduct >= 0)
        result = 1;
    else 
        result = 0;

    return result;
}

/* Print statistics of sub hash function */
void hCosin::print(void){

    if(this->r == NULL)
        cout << "Invalid cosin function\n";
    else{
        cout << "Cosin(h) id: " << this->id << "\n";
        cout << "Statistics of r: \n";
        this->r->print();
    }
}
// Petropoulakis Panagiotis
