#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "hasFunction.h"
#include "../item/item.h"
#include "../myLimits/myLimits.h"

using namespace std;

/////////////////////////////////////////////////////////
/* Implementation of abstract sub-hash functions class */
/* Note: No default dehaviors - behave like inrerface  */
/////////////////////////////////////////////////////////

H::~H(){}

/////////////////////////////////////////////////////////
/* Implementation of sub euclidean hash function class */
/////////////////////////////////////////////////////////

HEuclidean::HEuclidean(string& id, int& dim){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM){
        this->v = NULL;
    }
    else{ 
        /* Set random engine */
        static unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        static default_random_engine generator(seed);
        
        /* Set uniform and standard distribution */
        static uniform_real_distribution<double> uniformDist(0,W);
        static normal_distribution<double> normalDist(0,1);
        
        /* Temporary vector for item v */
        vector<double> components(dim);
        int i = 0;

        /* Fix id */
        this->id = id;

        /* Pick a random t - uniform distribution */
        this->t = uniformDist(generator);

        /* Fix item */
        for(i = 0; i < dim; i++)
            components[i] = normalDist(generator);

        int status;
        this->v = new Item(id,components,status);
    }
}

HEuclidean::~HEuclidean(){
    if(v != NULL)
        delete v;
}

int HEuclidean::hash(Item& p, int& status){

}

void HEuclidean::print(void){

    if(this->v == NULL)
        cout << "Invalid HEuclidean\n";
    else{
        cout << "HEuclidean id: " << this->id << "\n";
        cout << "Value of t: " << this->t  << "\n"; 
        cout << "Statistics of v: \n";
        
        this->v->print();
    }
}


// Petropoulakis Panagiotis
