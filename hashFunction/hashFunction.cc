#include <iostream>
#include <vector>
#include <math.h>
#include "hashFunction.h"
#include "../item/item.h"
#include "../utils/utils.h"
#include "../myLimits/myLimits.h"

using namespace std;

/////////////////////////////////////////////////////////
/* Implementation of abstract sub-hash functions class */
/* Note: No default dehaviors - behave like inrerface  */
/////////////////////////////////////////////////////////

h::~h(){}

/////////////////////////////////////////////////////////
/* Implementation of sub euclidean hash function class */
/////////////////////////////////////////////////////////

hEuclidean::hEuclidean(string& id, int& dim){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM){
        this->v = NULL;
    }
    else{ 
        /* Temporary vector for item v */
        vector<double> components(dim);
        int i = 0, status;

        /* Fix id */
        this->id = id;

        /* Pick a random t - uniform distribution */
        this->t = getRandom(0);

        /* Fix item - Pick random float in standard distribution */
        for(i = 0; i < dim; i++)
            components[i] = getRandom(1);

        /* Create item v */
        this->v = new Item(id,components,status);
        if(status != 0){
            delete this->v;
            this->v = NULL;
        } 
    }
}

/* Destructor */
hEuclidean::~hEuclidean(){
    if(v != NULL)
        delete v;
}

/* Calculate hash value of given p item */
/* h(p) = floor((p . v + t) / W)        */
int hEuclidean::hash(Item& p, int& status){
    float innerProduct;
    int result;

    status = 0;
    if(this->v == NULL){
        status = -3;
        return -1;
    }

    innerProduct = (float)p.innerProduct(*(this->v),status);
    if(status != 0)
        return -1;

    result = floor((innerProduct + this->t) / W);

    /* Truncate negative values */
    result = abs(result);

    return result;
}

/* Compare given sub hash functions */
/* Equal: 0                         */
/* Not equal: 1                     */
int hEuclidean::compare(hEuclidean& x, int& status){
    
    status = 0;

    if(this->v == NULL || x.v == NULL)
        status = -1;
    else{
       if((this->t == x.t) && this->v == x.v)
           return 0;
       else 
           return 1;
    }

}

/* Can't compare different hash functions */
int hEuclidean::compare(hCosin& x, int& status){
    status = -1;
    return -1;
}

/* Print statistics of sub hash function */
void hEuclidean::print(void){

    if(this->v == NULL)
        cout << "Invalid euclidean sub hash function\n";
    else{
        cout << "Euclidean(h) id: " << this->id << "\n";
        cout << "Value of t: " << this->t  << "\n"; 
        cout << "Statistics of v: \n"; 
        this->v->print();
    }
}

/////////////////////////////////////////////////////
/* Implementation of sub cosin hash function class */
/////////////////////////////////////////////////////

hCosin::hCosin(string& id, int& dim){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM){
        this->r = NULL;
    }
    else{ 
        /* Temporary vector for item r */
        vector<double> components(dim);
        int i = 0, status;

        /* Fix id */
        this->id = id;

        /* Fix item - Pick random float in standard distribution */
        for(i = 0; i < dim; i++)
            components[i] = getRandom(1);

        this->r = new Item(id,components,status);
        if(status != 0){
            delete this->r;
            this->r = NULL;
        } 
    }
}

/* Destructor */
hCosin::~hCosin(){
    if(r != NULL)
        delete r;
}

/* Calculate hash value of given p item */
/* h(p) = 1, if r.p >= 0                */
/* h(p) = 0, if r.p < 0                 */
int hCosin::hash(Item& p, int& status){
    float innerProduct;
    int result;

    status = 0;
    if(this->r == NULL){
        status = -3;
        return -1;
    }

    innerProduct = (float)p.innerProduct(*(this->r),status);
    if(status != 0)
        return -1;

    if(innerProduct >= 0)
        result = 1;
    else 
        result = 0;

    return result;
}

/* Compare given sub hash functions */
/* Equal: 0                         */
/* Not equal: 1                     */
int hCosin::compare(hCosin& x, int& status){
    
    status = 0;

    if(this->r == NULL || x.r == NULL)
        status = -1;
    else{
       if(this->r == x.r)
           return 0;
       else 
           return 1;
    }
}

/* Can't compare different hash functions */
int hCosin::compare(hEuclidean& x, int& status){
    status = -1;
    return -1;
}

/* Print statistics of sub hash function */
void hCosin::print(void){

    if(this->r == NULL)
        cout << "Invalid cosin sub hash function\n";
    else{
        cout << "Cosin(h) id: " << this->id << "\n";
        cout << "Statistics of r: \n";
        this->r->print();
    }
}
/* Abstract class for has function */

/////////////////////////////////////////////////////////
/* Implementation of abstract hash function class      */
/* Note: No default dehaviors - behave like inrerface  */
/////////////////////////////////////////////////////////

hashFunction::~hashFunction(){}

/////////////////////////////////////////////////////
/* Implementation of euclidean hash function class */
/////////////////////////////////////////////////////

hashFunctionEuclidean::hashFunctionEuclidean(string& id, int& dim, int& k, int& tableSize):tableSize(tableSize),k(k){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM || k <= 0 || k > MAX_K || M > MAX_M || M < MIN_M || tableSize <= 0){
        this->k = -1;
    }
    else{ 
        int i,j,status;
        hEuclidean* newFunc;
        
        /* Set size of R */
        this->R.reserve(k);

        /* Set size of H */
        this->H.reserve(k);

        for(i = 0; i < this->k; i++){
            newFunc = new hEuclidean(id,dim);

            for(j = 0; j < this->k; j++){
            
                /* Truncate same sub hash functions */
                if(this->H[i]->compare(*newFunc,status) == 0){
                    delete newFunc;
                    break;
                }
            } // End for

            /* Add function */
            if(j == this->k)
                this->H[i] = newFunc;
        } // End for

        /* Pick random R values */
        for(i = 0; i < this->k; i++)
            this->R[i] = getRandom(1);
    }
}

/* Destructor */
hashFunctionEuclidean::~hashFunctionEuclidean(){
    if(this->k != -1){
        int i;
        
        for(i = 0; i < this->k; i++)
            delete this->H[i];
    }
}

/* Calculate hash value of given p item                */
/* F(p) = [(r1h1(p)+...+rkhk(p)) mod M] mod table size */ 
int hashFunctionEuclidean::hash(Item& p, int& status){
    int result = 0,i,tmp;

    status = 0;    
    if(this->k != -1){
        status = -3;
        return -1;
    }

    /* Calculate F(p) */
    for(i = 0; i < k; i++){
        result += this->H[i]->hash(p,status) * this->R[i];
        if(result != 0)
            return -1;
 
    }

    return result;
}

/* Calculate hash value of given p item         */
/* G(p) = h1(p).h2(p)...hk(p) -> Concatenation  */
int hashFunctionEuclidean::hashLevel2(Item& p, int& status){
    int result = 0,i;

    status = 0;    
    if(this->k != -1){
        status = -3;
        return -1;
    }

    /* Calculate G(p) */
    for(i = 0; i < k; i++){
        result = result * 10 * (i + 1) + this->H[i]->hash(p,status);     
        if(result != 0)
            return -1;
    }

    return result;
}

/* Print statistics of sub hash function */
void hashFunctionEuclidean::print(void){

    if(this->k == -1)
        cout << "Invalid cosin hash function\n";
    else{
        int i;
        
        cout << "Euclidean hash function id: " << this->id << "\n";
        cout << "Value of k: " << this->k << "\n";
        cout << "Value of table size: " << this->tableSize << "\n"; 
        
        cout << "Values of ri: ";
        for(i = 0; i < this->k; i++)
            cout << this->R[i] << " ";
        cout << "\n";

        cout << "Statistics of sub hash functions: \n";
        
        for(i = 0; i < this->k; i++)
            this->H[i]->print();
    }
}

/////////////////////////////////////////////////
/* Implementation of cosin hash function class */
/////////////////////////////////////////////////

hashFunctionCosin::hashFunctionCosin(string& id, int& dim, int& k):k(k){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM || k <= 0 || k > MAX_K){
        this->k = -1;
    }
    else{ 
        int i,j,status;
        hCosin* newFunc;
       
        /* Set size of H */
        this->H.reserve(k);

        for(i = 0; i < this->k; i++){
            newFunc = new hCosin(id,dim);
            for(j = 0; j < this->k; j++){
            
                /* Truncate same sub hash functions */
                if(this->H[i]->compare(*newFunc,status) == 0){
                    delete newFunc;
                    break;
                }
            } // End for

            /* Add function */
            if(j == this->k)
                this->H[i] = newFunc;
        } // End for
    }
}

/* Destructor */
hashFunctionCosin::~hashFunctionCosin(){
    if(this->k != -1){
        int i;
        
        for(i = 0; i < this->k; i++)
            delete this->H[i];
    }
}

/* Calculate hash value of given p item         */
/* G(p) = h1(p).h2(p)...hk(p) -> Concatenation  */
int hashFunctionCosin::hash(Item& p, int& status){
    int result = 0,i;

    status = 0;    
    if(this->k == -1){
        status = -3;
        return -1;
    }

    /* Calculate G(p) */
    for(i = 0; i < k; i++){
        result = result * 10 * (i + 1) + this->H[i]->hash(p,status);     
        if(result != 0)
            return -1;
    }

    return result;
}

/* There isn't level 2 hashing in cosin */
int hashFunctionCosin::hashLevel2(Item& p, int& status){
    status = -1;
    return -1;
}

/* Print statistics of sub hash function */
void hashFunctionCosin::print(void){

    if(this->k == -1)
        cout << "Invalid cosin hash function\n";
    else{
        int i;
        
        cout << "Cosin has function id: " << this->id << "\n";
        cout << "Value of k: " << this->k << "\n";
        cout << "Statistics of sub hash functions: \n";
        
        for(i = 0; i < this->k; i++)
            this->H[i]->print();
    }
}

// Petropoulakis Panagiotis
