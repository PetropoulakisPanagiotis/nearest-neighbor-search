#pragma once

/* Set defines */
#define W 4 // Window size

/* Set limits */
#define MAX_DIM 20000 // Max dimension
#define MAX_K 40 // Max number of sub hash functions
#define MIN_K 1
#define MAX_W 500 // Max window size
#define MIN_W 2
#define MY_MAX_RANDOM 20
#define MY_MIN_RANDOM -20

/* Errors */
typedef enum errorCode{
    SUCCESS,
    INVALID_INDEX, 
    INVALID_DIM,  
    INVALID_HASH_FUNCTION,
    INVALID_COMPARE,
    SUM_OVERFLOW,
    SUB_OVERFLOW,
    MULT_OVERFLOW,
    DIV_OVERFLOW,
    ALLOCATION_FAILED
}errorCode;

///////////////////////
/* Usefull functions */
///////////////////////

/* Get random number - standard or uniform distribution */
float getRandom(int type, int w = 4);

/* My mod functions. Works also with negative values */
int myMod(int x, int y);

/* Detect overflow in given operation*/
double mySumDouble(double x, double y, errorCode& status);
double mySubDouble(double x, double y, errorCode& status);
double myMultDouble(double x, double y, errorCode& status);
double myDivDouble(double x, double y, errorCode& status);

int mySumInt(int x, int y, errorCode& status);
int mySubInt(int x, int y, errorCode& status);
int myMultInt(int x, int y, errorCode& status);
int myDivInt(int x, int y, errorCode& status);

/* Print type of error */
void printError(errorCode& status);
// Petropoulakis Panagiotis
