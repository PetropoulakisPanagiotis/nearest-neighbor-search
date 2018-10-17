#include <iostream>
#include <vector>
#include "hashFunction.h"
#include "../item/item.h"
#include "../utils/utils.h"

using namespace std;

int main(void){
    hashFunction* f;

    f = new hashFunctionEuclidean(4,3,10);

    f->print();

    delete f;

    return 0;
}
