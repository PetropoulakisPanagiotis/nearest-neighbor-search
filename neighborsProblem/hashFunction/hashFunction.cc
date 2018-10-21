#include <iostream>
#include <vector>
#include <cmath>
#include "hashFunction.h"
#include "../item/item.h"
#include "../utils/utils.h"

using namespace std;

/////////////////////////////////////////////////////////
/* Implementation of abstract sub hash functions class */
/* Note: No default dehaviors - behave like inrerface  */
/////////////////////////////////////////////////////////

h::~h(){}

/////////////////////////////////////////////////////////
/* Implementation of sub euclidean hash function class */
/////////////////////////////////////////////////////////

int hEuclidean::count = 0;

hEuclidean::hEuclidean(int dim, int w):w(w){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM || w < MIN_W || w > MAX_W){
        this->v = NULL;
    }
    else{ 
        /* Temporary vector for item v */
        int i = 0;
        vector<double> components(dim);
        errorCode status;

        /* Fix id */
        this->id = "hEuclidean_" + to_string(this->count);
        this->count += 1;

        /* Pick a random t - uniform distribution */
        this->t = getRandom(0,this->w);

        /* Fix item - Pick random float in standard distribution */
        for(i = 0; i < dim; i++)
            components[i] = getRandom(1);

        /* Create item v */
        this->v = new Item(components, status);
        if(status != 0){
            delete this->v;
            this->v = NULL;
        }

        if(this->v == NULL)
            status = ALLOCATION_FAILED;
    }
}

/* Destructor */
hEuclidean::~hEuclidean(){
    if(v != NULL){
        this->count -= 1;
        delete v;
    }
}

/* Calculate hash value of given p item */
/* h(p) = floor((p . v + t) / W)        */
int hEuclidean::hash(Item& p, errorCode& status){
    double innerProduct,tempSum,tempDiv;
    int result;

    status = SUCCESS;
    if(this->v == NULL){
        status = INVALID_HASH_FUNCTION;
        return -1;
    }

    innerProduct = p.innerProduct(*(this->v), status);
    if(status != SUCCESS)
        return -1;

    tempSum = mySumDouble(innerProduct, this->t, status);
    if(status != SUCCESS)
        return -1;

    tempDiv = myDivDouble(tempSum, this->w, status);
    if(status != SUCCESS)
        return -1;

    result = floor(tempDiv);

    return result;
}

/* Compare given sub hash functions */
/* Discard id                       */
/* Equal: 0                         */
/* Not equal: 1                     */
int hEuclidean::compare(hEuclidean& x, errorCode& status){
    
    status = SUCCESS;
    if(this->v == NULL || x.v == NULL){
        status = INVALID_HASH_FUNCTION;
        return -1;
    }
    else{
       if((this->v->compare(*(x.v),status) == 0) && this->t == x.t)
           return 0;
       else 
           return 1;
    }
}

/* Can't compare different hash functions */
int hEuclidean::compare(hCosin& x, errorCode& status){
    status = INVALID_COMPARE;
    return -1;
}

/* Get number of sub hash functions */
int hEuclidean::getCount(void){
    return this->count;
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

int hCosin::count = 0;

hCosin::hCosin(int dim){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM){
        this->r = NULL;
    }
    else{ 
        /* Temporary vector for item r */
        vector<double> components(dim);
        int i = 0;
        errorCode status;

        /* Fix id */
        this->id = "hCosin_" + to_string(this->count); 
        this->count += 1;

        /* Fix item - Pick random float in standard distribution */
        for(i = 0; i < dim; i++)
            components[i] = getRandom(1);

        this->r = new Item(components, status);
        if(status != 0){
            delete this->r;
            this->r = NULL;
        }

        if(this->r == NULL)
            status = ALLOCATION_FAILED;

    }
}

/* Destructor */
hCosin::~hCosin(){
    if(r != NULL){
        this->count -= 1;
        delete r;
    }
}

/* Calculate hash value of given p item */
/* h(p) = 1, if r.p >= 0                */
/* h(p) = 0, if r.p < 0                 */
int hCosin::hash(Item& p, errorCode& status){
    double innerProduct;
    int result;

    status = SUCCESS;
    if(this->r == NULL){
        status = INVALID_HASH_FUNCTION;
        return -1;
    }

    innerProduct = p.innerProduct(*(this->r), status);
    if(status != SUCCESS)
        return -1;

    if(innerProduct >= 0)
        result = 1;
    else 
        result = 0;

    return result;
}

/* Compare given sub hash functions */
/* Discard id                       */
/* Equal: 0                         */
/* Not equal: 1                     */
int hCosin::compare(hCosin& x, errorCode& status){
    
    status = SUCCESS;

    if(this->r == NULL || x.r == NULL){
        status = INVALID_HASH_FUNCTION;
        return -1;
    }
    else{
       if(this->r->compare(*(x.r), status) == 0)
           return 0;
       else 
           return 1;
    }
}

/* Can't compare different hash functions */
int hCosin::compare(hEuclidean& x, errorCode& status){
    status = INVALID_COMPARE;
    return -1;
}

/* Get number of sub hash functions */
int hCosin::getCount(void){
    return this->count;
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

/////////////////////////////////////////////////////////
/* Implementation of abstract hash function class      */
/* Note: No default dehaviors - behave like inrerface  */
/////////////////////////////////////////////////////////

hashFunction::~hashFunction(){}

/////////////////////////////////////////////////////
/* Implementation of euclidean hash function class */
/////////////////////////////////////////////////////

int hashFunctionEuclidean::count = 0;

hashFunctionEuclidean::hashFunctionEuclidean(int dim, int k, int w, int tableSize):k(k),w(w),tableSize(tableSize){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM || k < MIN_K || k > MAX_K || tableSize <= 0 || w < MIN_W || w > MAX_W){
        this->k = -1;
    }
    else{ 
        int i,j;
        errorCode status;
        hEuclidean* newFunc;
        
        this->id = "EuclideanHash_" + to_string(this->count); 
        this->count += 1;

        /* Set size of R */
        this->R.reserve(k);

        /* Set size of H */
        this->H.reserve(k);

        /* Pick k hash(h) functions */
        for(i = 0; i < this->k; i++){
            newFunc = new hEuclidean(dim,w);
            if(newFunc == NULL){
                status = ALLOCATION_FAILED;
                this->k = -1;
                break;
            }


            for(j = 0; j < i; j++){
            
                /* Truncate same sub hash functions */
                if(this->H[j]->compare(*newFunc, status) == 0){
                    delete newFunc;
                    break;
                }

                if(status != SUCCESS){
                    delete newFunc;
                    this->k = -1;
                    break;
                }

            } // End for
            
            if(status != SUCCESS)
                break;

            /* Add function */
            if(j == i)
                this->H[i] = newFunc;
            else
                i -= 1;
        } // End for

        /* Delete remaining h functions */
        if(status != SUCCESS){
            for(j = 0; j < i; j++)
                delete this->H[j];
            
            return;
        }

        /* Pick random R values */
        for(i = 0; i < this->k; i++)
            this->R[i] = (int)getRandom(2);
    }
}

/* Destructor */
hashFunctionEuclidean::~hashFunctionEuclidean(){
    if(this->k != -1){
        int i;
        
        this->count -= 1;

        for(i = 0; i < this->k; i++)
            delete this->H[i];
    }
}

/* Calculate hash value of given p item                */
/* F(p) = [(r1h1(p)+...+rkhk(p)) mod M] mod table size */ 
int hashFunctionEuclidean::hash(Item& p, errorCode& status){
    int result = 0,i,tmpMult;

    status = SUCCESS;    
    if(this->k == -1){
        status = INVALID_HASH_FUNCTION;
        return -1;
    }

    /* Calculate F(p) */
    for(i = 0; i < k; i++){
        tmpMult = myMultInt(this->H[i]->hash(p, status), this->R[i], status);
        if(status != SUCCESS)
            return -1;

        result = mySumInt(result, tmpMult, status);
        if(status != SUCCESS)
            return -1;
    }

    result = myMod(result, tableSize);

    return result;
}

/* Calculate hash value of given p item         */
/* G(p) = h1(p).h2(p)...hk(p) -> Concatenation  */
int hashFunctionEuclidean::hashLevel2(Item& p, errorCode& status){
    int result = 0, i, tempMult;

    status = SUCCESS;    
    if(this->k == -1){
        status = INVALID_HASH_FUNCTION;
        return -1;
    }

    /* Calculate G(p) */
    for(i = 0; i < k; i++){

        tempMult = myMultInt(result, 10, status);
        if(status != SUCCESS)
            return -1;

        result = mySumInt(tempMult, this->H[i]->hash(p, status), status);
        if(status != SUCCESS)
            return -1;
    }

    return result;
}

/* Compare given hash functions */
/* Discard id                   */
/* Equal: 0                     */
/* Not equal: 1                 */
int hashFunctionEuclidean::compare(hashFunctionEuclidean& x, errorCode& status){
    int i;

    status = SUCCESS;

    if(this->k == -1 || x.k == -1){
        status = INVALID_HASH_FUNCTION;
        return -1;
    }

    if(this->k != x.k || this->tableSize != x.tableSize)
        return 1;

    /* Compare h functions */
    for(i = 0; i < this->k; i++)
        if(this->H[i]->compare(*(x.H[i]), status) != 0)
            return 1;
    
    if(status != SUCCESS)
        return -1;

    /* Compare r values */
    for(i = 0; i < this->k; i++)
        if(this->R[i] != x.R[i])
            return 1;

    return 0;
}

/* Can't compare different hash functions */
int hashFunctionEuclidean::compare(hashFunctionCosin& x, errorCode& status){
    status = INVALID_COMPARE;
    return -1;
}

/* Get number of euclidean has functions */
int hashFunctionEuclidean::getCount(void){
    return this->count;
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
        cout << "\n\n";

        cout << "Statistics of sub hash functions: \n\n";
        
        for(i = 0; i < this->k; i++)
            this->H[i]->print();
    }
}

/////////////////////////////////////////////////
/* Implementation of cosin hash function class */
/////////////////////////////////////////////////

int hashFunctionCosin::count = 0;

hashFunctionCosin::hashFunctionCosin(int dim, int k):k(k){
    /* Check parameters */
    if(dim <= 0 || dim > MAX_DIM || k <= 0 || k > MAX_K){
        this->k = -1;
    }
    else{ 
        int i,j;
        errorCode status;
        hCosin* newFunc;
       
        /* Set name */
        this->id = "CosinHash_" + to_string(this->count);
        this->count += 1;

        /* Set size of H */
        this->H.reserve(k);

        for(i = 0; i < this->k; i++){
            newFunc = new hCosin(dim);
            if(newFunc == NULL){
                status = ALLOCATION_FAILED;
                this->k = -1;
                break;  
            }

            for(j = 0; j < i; j++){
            
                /* Truncate same sub hash functions */
                if(this->H[j]->compare(*newFunc, status) == 0){
                    delete newFunc;
                    break;
                }
            
                if(status != SUCCESS){
                    delete newFunc;
                    this->k = -1;
                    break;
                }
            
            } // End for

            if(status != SUCCESS)
                break;

            /* Add function */
            if(j == i)
                this->H[i] = newFunc;
            else 
                i -= 1;
        } // End for

        /* Delete remaining h functions */
        if(status != SUCCESS)
            for(j = 0; j < i; j++)
                delete this->H[j];
    }
}

/* Destructor */
hashFunctionCosin::~hashFunctionCosin(){
    if(this->k != -1){
        int i;
        
        this->count -= 1;

        for(i = 0; i < this->k; i++)
            delete this->H[i];
    }
}

/* Calculate hash value of given p item         */
/* G(p) = h1(p).h2(p)...hk(p) -> Concatenation  */
int hashFunctionCosin::hash(Item& p, errorCode& status){
    int result = 0, i, tempMult;

    status = SUCCESS;    
    if(this->k == -1){
        status = INVALID_HASH_FUNCTION;
        return -1;
    }

    /* Calculate G(p) */
    for(i = 0; i < k; i++){

        tempMult = myMultInt(result, 10, status);
        if(status != SUCCESS)
            return -1;

        result = mySumInt(tempMult, this->H[i]->hash(p, status), status);
        if(status != SUCCESS)
            return -1;
    }

    return result;
}

/* There isn't level 2 hashing in cosin */
int hashFunctionCosin::hashLevel2(Item& p, errorCode& status){
    status = INVALID_COMPARE;
    return -1;
}

/* Compare given hash functions */
/* Discard id                   */
/* Equal: 0                     */
/* Not equal: 1                 */
int hashFunctionCosin::compare(hashFunctionCosin& x, errorCode& status){
    int i;

    status = SUCCESS;

    if(this->k == -1 || x.k == -1){
        status = INVALID_HASH_FUNCTION;
        return -1;
    }

    if(this->k != x.k)
        return -1;

    /* Compare h functions */
    for(i = 0; i < this->k; i++)
        if(this->H[i]->compare(*(x.H[i]), status) != 0)
            return 1;

    return 0;
}

/* Can't compare different hash functions */
int hashFunctionCosin::compare(hashFunctionEuclidean& x, errorCode& status){
    status = INVALID_COMPARE;
    return -1;
}

/* Get total cosin functions */
int hashFunctionCosin::getCount(void){
    return this->count;
}

/* Print statistics of sub hash function */
void hashFunctionCosin::print(void){

    if(this->k == -1)
        cout << "Invalid cosin hash function\n";
    else{
        int i;
        
        cout << "Cosin has function id: " << this->id << "\n";
        cout << "Value of k: " << this->k << "\n\n";
        cout << "Statistics of sub hash functions: \n\n";
        
        for(i = 0; i < this->k; i++)
            this->H[i]->print();
    }
}

// Petropoulakis Panagiotis
