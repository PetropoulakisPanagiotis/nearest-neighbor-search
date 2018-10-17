#include <iostream>
#include <random>
#include <chrono>
#include <limits>
#include <cmath>
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

/* Sum given numbers - check for overflow */
double mySum(double x, double y, errorCode& status){

    status = SUM_OVERFLOW;

    if(y > 0 && x > numeric_limits<double>::max() - y)
        return 0;
   
    if(y < 0 && x < numeric_limits<double>::lowest() - y)
        return 0;
  
    
    status = SUCCESS;
    
    return x + y;
}

/* Subtract given numbers - check for overflow */
double mySub(double x, double y, errorCode& status){

    status = SUB_OVERFLOW;

    if(y > 0 && x < numeric_limits<double>::lowest() + y)
        return 0;
   
    if(y < 0 && x > numeric_limits<double>::max() + y)
        return 0;  
    
    status = SUCCESS;
    
    return x - y;
}

/* Multiply given numbers - check for overflow */
double myMult(double x, double y, errorCode& status){

    status = MULT_OVERFLOW;

    if(x > 0 && y > 0)
        if(x > numeric_limits<double>::max() / y)
            return 0;
    
    if(x > 0 && y < 0)
        if(y < numeric_limits<double>::lowest() / x)
            return 0;
    
    if(x < 0 && y > 0)
        if(x > numeric_limits<double>::lowest() / y)
            return 0;

    if(x < 0 && y < 0)
        if(y < numeric_limits<double>::max() / x)
            return 0;

    status = SUCCESS;
    
    return x * y;
}

/* Divide given numbers - check for overflow */
double myDiv(double x, double y, errorCode& status){

    status = DIV_OVERFLOW;

    if( y == 0 || (a == numeric_limits<double>::lowest && y == -1)
        return 0;  
    
    status = SUCCESS;
    
    return x / y;
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
    
        case(SUM_OVERFLOW):
            cout << "Can't add given numbers. Possible overflow occures\n";
            break;
        
        case(SUB_OVERFLOW):
            cout << "Can't subtract given numbers. Possible overflow occures\n";
            break;
        
        case(MULT_OVERFLOW):
            cout << "Can't multiply given numbers. Possible overflow occures\n";
            break;
        
        case(DIV_OVERFLOW):
            cout << "Can't divide given numbers. Possible overflow occures\n";
            break;
    } // End switch
}

// Petropoulakis Panagiotis
