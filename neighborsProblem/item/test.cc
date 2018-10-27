#include <iostream>
#include "item.h"
#include "../utils/utils.h"
#include <vector>

using namespace std;

int main(void){ 
    Item* x;
    Item* y;
    vector<double> v1(50);
    vector<double> v2(500);
    int i;
    errorCode status;
    double comp;
    Item z;
    string str="hi";

    for(i = 0; i < 50; i++)
        v1.push_back(1);

    x = new Item(str,v2,status);
    y = new Item(str,v1,status);
  
    cout << x->size() << "\n";

    delete x;
    delete y;
    return 0;
}
