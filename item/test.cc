#include <iostream>
#include "item.h"
#include <vector>

using namespace std;

int main(void){ 
    Item* x;
    Item* y;
    vector<double> v1(5);
    vector<double> v2(5);
    int i,status;
    double comp;
    Item z;
    string str="hi";

    for(i = 0; i < 5; i++){
        v2[i] = i;
        v1[i] = i + 10;
    }

    x = new Item(str,v2,status);
    y = new Item(str,v1,status);
   
    cout << x->cosinDist(*y,status);
    cout << "\n" << status;
   
    delete x;
    delete y;
    return 0;
}
