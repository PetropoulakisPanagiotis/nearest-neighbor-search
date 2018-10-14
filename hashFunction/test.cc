#include <iostream>
#include <vector>
#include "hashFunction.h"
#include "../item/item.h"

using namespace std;

int main(void){
    h* h;
    string str = "hi";
    int x = 5,i, status;
    Item* p;
    vector<double> ve(5); 
    
    h = new hCosin(str,x);

    for(i = 0; i < 5;i++)
            ve[i] = i + 1;

    p = new Item(str,ve,status);

    h->print();

    cout << h->hash(*p,status) << "\n";
    p->print();


    delete h;
    delete p;
    return 0;
}
