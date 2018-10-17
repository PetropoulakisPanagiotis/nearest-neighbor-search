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
double mySumDouble(double,double,errorCode&);
double mySubDouble(double,double,errorCode&);
double myMultDouble(double,double,errorCode&);
double myDivDouble(double,double,errorCode&);

int mySumInt(int,int,errorCode&);
int mySubInt(int,int,errorCode&);
int myMultInt(int,int,errorCode&);
int myDivInt(int,int,errorCode&);

/* Print type of error */
void printError(errorCode);
// Petropoulakis Panagiotis
