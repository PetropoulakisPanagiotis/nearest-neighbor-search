#include <iostream>
#include <random>
#include <chrono>
#include "utils.h"
#include "../myLimits/myLimits.h"

using namespace std;

/* Get random float number */
/* Type 0: uniform float   */
/* Type 1: standard float  */
float getRandom(int type){
    /* Set random engine */
    static unsigned seed = chrono::system_clock::now().time_since_epoch().count();    
    static default_random_engine generator(seed);
        
    /* Set uniform and standard distribution */
    static uniform_real_distribution<float> uniformDist(0,W);
    static normal_distribution<float> normalDist(0,1);

    if(type == 0)
        return uniformDist(generator);
    else
        return normalDist(generator);
}

// Petropoulakis Panagiotis
