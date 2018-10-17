#include <iostream>
#include <random>
#include <chrono>
#include "utils.h"
#include "myLimits.h"

using namespace std;

/* Get random float number */
/* Type 0: uniform float   */
/* Type 1: standard float  */
/* Type 2: uniform float   */
float getRandom(int type){
    /* Set random engine */
    static unsigned seed = chrono::system_clock::now().time_since_epoch().count();    
    static default_random_engine generator(seed);
        
    /* Set uniform and standard distribution */
    static uniform_real_distribution<float> uniformDist1(0,W); // [0,W)
    static uniform_real_distribution<float> uniformDist2(MY_MIN_RANDOM,MY_MAX_RANDOM);
    static normal_distribution<float> normalDist(0,1); 

    if(type == 0)
        return uniformDist1(generator);
    else if(type == 1)
        return normalDist(generator);
    else
        return uniformDist2(generator);
}

/* Get mod of given number */
int getMod(int x, int y){

    if(x >= 0)
        return (x % y);
    else{
        if(abs(x) >= y)   
            return ((((-1) * x) % y) % y);
        else 
            return (x + y);
    }
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
        
        case(INVALID_HASH_FUNCTION):
            cout << "Hash function is broken. Has not been initialized properly\n";
            break;

        case(INVALID_COMPARE):
            cout << "Can't compare different types of hash functions\n";
            break;
    } // End switch
}

// Petropoulakis Panagiotis
