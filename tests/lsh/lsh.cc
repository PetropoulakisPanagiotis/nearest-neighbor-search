#include <iostream>
#include <vector>
#include <list>
#include "../../neighborsProblem/utils/utils.h"
#include "../../neighborsProblem/fileHandler/fileHandler.h"
#include "../../neighborsProblem/item/item.h"
#include "../../neighborsProblem/model/model.h"

using namespace std;

int main(void){
    vector<Item> x;
    list<string> y;
    errorCode error;

    readDataSet("../dataSets//input_small",x,y,error,1);
    
    
    
    
    
    
    
    
    
    
    
    
    /*
    model* myModel;
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

    myModel = new lshEuclidean(3,3,3,1,status);
 
    for(i = 0; i < 10; i++)
        vec[i] = i + 2;

    it = new Item(vec,status);
    
    l.push_back(*it);
    myModel->fit(l,status);
   
    myModel->print();

    
    delete it;
    delete myModel;
    */

    return 0;
}

// Petropoulakis Panagiotis
