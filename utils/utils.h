#pragma once

/* Errors */
typedef enum errorCode{
    SUCCESS,
    INVALID_INDEX, // Invalid index parameter
    INVALID_DIM, // Invalid dimension 
    INVALID_HASH_FUNCTION,
    INVALID_COMPARE
}errorCode;

///////////////////////
/* Usefull functions */
///////////////////////

/* Get random number - standard or uniform distribution */
float getRandom(int);

/* My mod functions. Works also with negative values */
int getMod(int,int);

/* Print type of error */
void printError(errorCode);
// Petropoulakis Panagiotis
