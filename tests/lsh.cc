#include <iostream>
#include "../utils/utils.h"
#include "../neighborsProblem/neighborsProblem.h"

int main(void){
    neighborsProblem* model;
    errorCode status;
    model = new lshEuclidean(-1,1,1,1,status);

    model->print();

    delete model;
    return 0;
}
