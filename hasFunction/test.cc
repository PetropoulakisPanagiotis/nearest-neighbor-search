#include <iostream>
#include "hasFunction.h"

int main(void){
    H* h, *t;
    std::string str = "hi";
    int x = 3;
    h = new HEuclidean(str,x);
    t = new HEuclidean(str,x);

    h->print();

    delete h;
    delete t;
    return 0;
}
