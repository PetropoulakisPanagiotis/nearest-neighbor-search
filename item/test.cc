#include <iostream>
#include "item.h"
#include "../utils/utils.h"
#include <vector>

using namespace std;

int main(void){ 
    Item* x;
    Item* y;
    vector<float> v1(5);
    vector<float> v2(5);
    int i;
    errorCode status;
    float comp;
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
