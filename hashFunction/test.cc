#include <iostream>
#include <vector>
#include <limits>
#include "hashFunction.h"
#include "../item/item.h"
#include "../utils/utils.h"

using namespace std;

int main(void){
    hashFunction* f;
    errorCode status;
    vector<double> vec(4);

    int ix;

    for(ix = 0; ix < 4; ix++)
        vec[ix] = ix + 1;

    Item* i = new Item(vec,status);
    i->print();

    f = new hashFunctionCosin(4,3);

    f->print();
    
    cout << "f:" << f->hash(*i,status) << "\n";
    cout << "g:" << f->hashLevel2(*i,status) << "\n";
    
    printError(status);
    
    
    delete f;
    delete i;

    return 0;
}
