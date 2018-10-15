#include <iostream>
#include <random>
#include <chrono>
#include "utils.h"
#include "myLimits.h"

using namespace std;

/* Get random float number */
/* Type 0: uniform float   */
/* Type 1: standard float  */
float getRandom(int type){
    /* Set random engine */
    static unsigned seed = chrono::system_clock::now().time_since_epoch().count();    
    static default_random_engine generator(seed);
        
    /* Set uniform and standard distribution */
    static uniform_real_distribution<float> uniformDist(0,W); // [0,W)
    static normal_distribution<float> normalDist(0,1); 

    if(type == 0)
        return uniformDist(generator);
    else
        return normalDist(generator);
}
/* Print type of error */
void printError(errorCode status){

    switch(status){
        case(SUCCESS):
            cout << "No error occured\n";
            break;

        case(INVALID_INDEX):
            cout << "Please give a valid index\n";
            break;
        
        case(INVALID_DIM):
            cout << "Please give a valid positive dimension up to: ";
            cout << MAX_DIM;
            cout << "\n";
            break;
        
        case(INVALID_HASH_FUNCTION);
            cout << "Hash function is broken. Has not been initialized properly\n";
            break;

        case(INVALID_COMPARE):
            cout << "Can't compare different types of hash functions\n";
            break;
    } // End switch
}

// Petropoulakis Panagiotis
