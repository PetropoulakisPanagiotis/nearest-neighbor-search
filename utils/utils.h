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
float getRandom(int type);

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
