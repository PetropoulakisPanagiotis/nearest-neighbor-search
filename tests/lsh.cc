#include <iostream>
#include <vector>
#include <list>
#include "../utils/utils.h"
#include "../neighborsProblem/neighborsProblem.h"
#include "../item/item.h"

using namespace std;

int main(void){
    neighborsProblem* model;
    errorCode status;
    Item* it;
    Item* p;
    vector<double> vec(10);
    list<Item> l;
    
    int i;
    for(i = 0; i < 10; i++)
        vec[i] = i + 1;

    it = new Item(vec,status);
    l.push_back(*it);
    delete it;

    model = new lshEuclidean(3,3,3,1,status);
 
    for(i = 0; i < 10; i++)
        vec[i] = i + 2;

    it = new Item(vec,status);
    
    l.push_back(*it);
    model->fit(l,status);
   
    model->print();

    
    delete it;
    delete model;
    
    return 0;
}
