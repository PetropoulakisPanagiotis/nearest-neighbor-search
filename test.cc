#include <iostream>
#include <limits>
#include "utils/utils.h"

using namespace std;

int main(void){
    errorCode status;

    cout << myMult(numeric_limits<double>::max(),3112121222211211231,status) << "\n";
    cout << status << "\n";
    
    return 0;
}
