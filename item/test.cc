#include <iostream>
#include "item.h"
#include <vector>

using namespace std;
int main(void){
    Item* x;
    vector<double> components(10);

    int i;

    for(i = 0; i < 10; i++)
        components[i] = i;

    string t = "hi";
    x = new Item(t,components);
    
    x->ItemPrint();
    components[0] = 12;

    x->ItemPrint();

    Item* y = new Item();

    y = x;
    y->ItemPrint();
    double p = 99;;
    int r = 1,s;
    x->ItemSetComponent(p,r,s);

    x->ItemPrint();

    y->ItemPrint();


    return 0;
}
