#pragma once

/* Errors */
typedef enum errorCode{
    SUCCESS,
    INVALID_INDEX, // Invalid index parameter
    INVALID_DIM, // Invalid dimension 
    INVALID_HASH_FUNCTION,
    INVALID_COMPARE,
    SUM_OVERFLOW,
    SUB_OVERFLOW,
    MULT_OVERFLOW,
    DIV_OVERFLOW
}errorCode;

///////////////////////
/* Usefull functions */
///////////////////////

/* Get random number - standard or uniform distribution */
float getRandom(int);

/* My mod functions. Works also with negative values */
int getMod(int,int);

/* Detect overflow in given operation*/
double mySum(double,double,errorCode&);
double mySub(double,double,errorCode&);
double myMult(double,double,errorCode&);
double myDiv(double,double,errorCode&);

/* Print type of error */
void printError(errorCode);
// Petropoulakis Panagiotis
