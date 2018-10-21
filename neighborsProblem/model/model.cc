#include <iostream>
#include <vector>
#include <list>
#include "../hashFunction/hashFunction.h"
#include "../item/item.h"
#include "../utils/utils.h"
#include "model.h"

using namespace std;

/////////////////////////////////////////////////////////
/* Implementation of abstract neighbors Problem        */
/* Note: No default dehaviors - behave like inrerface  */
/////////////////////////////////////////////////////////

model::~model(){}

///////////////////////////////////////////
/* Implementation of lsh euclidean class */
///////////////////////////////////////////

/* Default constructor */
lshEuclidean::lshEuclidean():l(5),k(4),w(200),coefficient(1/4),fitted(0){
    /* Set size of hash functions */
    int i;
    this->hashFunctions.reserve(this->l);
    for(i = 0; i < this->l; i++)
        this->hashFunctions[i] = NULL;

    /* Set size of hash tables */
    for(i = 0; i < this->l; i++)
        this->tables.push_back(vector<list<entry> >(this->l));
}

lshEuclidean::lshEuclidean(int l, int k, int w, float coefficient, errorCode& status):l(l),k(k),w(w),coefficient(coefficient),fitted(0){

    /* Check parameters */
    if(l < MIN_L || l > MAX_L || k < MIN_K || k > MAX_K || w < MIN_W || w > MAX_W || coefficient < MIN_C || coefficient > MAX_C){
        status = INVALID_PARAMETERS;
        this->k = -1;
    }
    else{
      
        /* Set size of hash functions */
        int i;
        this->hashFunctions.reserve(this->l);
        for(i = 0; i < this->l; i++)
            this->hashFunctions[i] = NULL;

        /* Set size of hash tables */
        for(i = 0; i < this->l; i++)
            this->tables.push_back(vector<list<entry> >(this->l));
    
    }
}

lshEuclidean::~lshEuclidean(){
 
    /* Check method */
    if(this->k == -1){
        return;
    }   
    
    int i;

    /* Delete hash functions */
    if(this->fitted == 1)
        for(i = 0; i < this->l; i++)
            delete this->hashFunctions[i];
}

/* Fix hash table, members of lsh euclidean and add given points in the hash tables */
void lshEuclidean::fit(list<Item>& points, errorCode& status){
    int i, j, k;
    int flag = 0; // Invalid points
    int pos; // Pos(line) in current hash table
    entry newEntry;

    /* Iteratiors */
    list<Item>::iterator iterPoints = points.begin(); // Iterate through points
    list<entry>::iterator iterEntries;  // Iterate through entries
   
    status = SUCCESS;

    /* Check method */
    if(this->k == -1){
        status = INVALID_METHOD;
        return;
    }

    /* Already fitted */
    if(this->fitted == 1){
        status = METHOD_ALREADY_USED;
        return;
    }

    /* Check parameters */
    if(points.size() > MAX_DIM || points.size() <= 0){
        status = INVALID_DIM;
        return;
    }

    /* Set members */
    this->n = points.size();
    if(this->n < MIN_POINTS || this->n > MAX_POINTS){
        status = INVALID_POINTS;
        return;
    }

    /* Set table size */
    this->tableSize = (int)(this->n * this->coefficient);
  
    /* Fix each table - Each table contains lists of entries */
    for(i = 0; i < this->l; i++)
        for(j = 0; j < this->tableSize; j++)
            this->tables[i].push_back(list<entry>());

    /* Set dimension */
    this->dim = iterPoints->getDim();
    if(this->dim <= 0){
        status = INVALID_DIM;
        return;
    }
    
    ////////////////////////
    /* Set hash functions */
    ////////////////////////

    /* Function that will be inserted in hash functions table */
    hashFunctionEuclidean* newFunc;  
    
    for(i = 0; i < this->l; i++){
        newFunc = new hashFunctionEuclidean(this->dim, this->k, this->w, this->tableSize);
        
        /* Truncate same hash functions */
        for(j = 0; j < i; j++){
            if(this->hashFunctions[j]->compare(*newFunc,status) == 0){
                delete newFunc;
                break;
            }
        } // End for
        
        if(i == j){
            this->hashFunctions[i] = newFunc; // Add hash function
        }
        else
            i -= 1;
    } // End for - Hash functions

    /////////////////////
    /* Set hash tables */
    /////////////////////

    /* Scan each table */
    for(i = 0; i < this->l; i++){
    
        /* Scan given points */
        for(iterPoints = points.begin(); iterPoints != points.end() ; iterPoints++){
            /* Check consistency of dim */
            if(this->dim != iterPoints->getDim()){
                status = INVALID_DIM;
                flag = 1;
                break;
            }
            
            /* Find position in hash table */
            pos = this->hashFunctions[i]->hash(*iterPoints,status);
            if(pos < 0 || pos >= tableSize){
                status = INVALID_HASH_FUNCTION;
                flag = 1;
                break;
            }
           
            /* Set new entry */
            newEntry.point = *(iterPoints);
            newEntry.valueG = this->hashFunctions[i]->hashLevel2(*iterPoints,status);
            
            /* Add point */
            this->tables[i][pos].push_back(newEntry);

        } // End for - Points

        if(flag == 1)
            break;
    } // End for - Hash tables
  
    /* Error occured - Clear structures */
    if(flag == 1){
       
        /* Clear points */
        for(i = 0; i < this->l; i++)
            for(j = 0; j < this->tableSize; j++)
                this->tables[i].clear();
 
        /* Clear hash functions */
        for(i = 0; i < this->l; i++)
            delete this->hashFunctions[i];      
    }
    else
        /* Method fitted */
        this->fitted = 1;
}

/* Find the radius neighbors of a given point */
void lshEuclidean::radiusNeighbors(Item& query, int radius, list<Item>& neighbors, list<double>* neighborsDistances, errorCode& status){
    int queryDim = query.getDim();
    int i, pos, valueG, found = 0, flag = 0;
    double minDist = -1; // Current minimum distance 
    double currDist; // Distance of a point in list
    list<entry>::iterator iter;
    list<entry>::iterator iterNearestNeighbor;

    status = SUCCESS;

    /* Check parameters */
    if(queryDim <= 0 || queryDim > MAX_DIM || queryDim != this->dim){
        status = INVALID_DIM;
        return;
    }

    /* Check model */
    if(this->fitted == 0){
        status = METHOD_UNFITTED;
        return;
    }

    if(this->k == -1){
        status = INVALID_METHOD;
        return;
    }

    /* Scan all tables */
    for(i = 0; i < this->l; i++){
    
        /* Find position in table */
        pos = this->hashFunctions[i]->hash(query,status);
        if(status != 0)
            return;

        /* Empty list */
        if(this->tables[i][pos].size() == 0)
            continue;

        /* Find value g for query */
        valueG = this->hashFunctions[i]->hashLevel2(query,status);
        if(status != 0)
            return;

        /* Scan list of specific bucket */
        for(iter = this->tables[i][pos].begin(); iter != this->tables[i][pos].end(); iter++){  

            /* Compare values g of query and current point */
            if(valueG != iter->valueG)
                continue;

            /* Find current distance */
            currDist = iter->point.euclideanDist(query,status);
            if(status != 0)
                return;
            
            /* First neighbor */
            if(flag == 0){
                minDist = currDist;
                iterNearestNeighbor = iter;
                
                found = 1;
                flag = 1;
            }

            /* Change min distance */
            else if(minDist > currDist){
                minDist = currDist;
                iterNearestNeighbor = iter;
            }

        } // End for - Scan list
    } // End for - Tables

    /* Nearest neighbor found */
    if(found == 1){
        nNeighbor = iterNearestNeighbor->point;
        if(neighborDistance != NULL)
            *neighborDistance = minDist;
    }
    else{
        nNeighbor.setId("Nearest neighbor not found");
        if(neighborDistance != NULL)
            *neighborDistance = minDist;
    }
}
/* Find the nearest neighbor of a given point */
void lshEuclidean::nNeighbor(Item& query, Item& nNeighbor, double* neighborDistance, errorCode& status){
    int queryDim = query.getDim();
    int i, pos, valueG, found = 0, flag = 0;
    double minDist = -1; // Current minimum distance 
    double currDist; // Distance of a point in list
    list<entry>::iterator iter;
    list<entry>::iterator iterNearestNeighbor;

    status = SUCCESS;

    /* Check parameters */
    if(queryDim <= 0 || queryDim > MAX_DIM || queryDim != this->dim){
        status = INVALID_DIM;
        return;
    }

    /* Check model */
    if(this->fitted == 0){
        status = METHOD_UNFITTED;
        return;
    }

    if(this->k == -1){
        status = INVALID_METHOD;
        return;
    }

    /* Scan all tables */
    for(i = 0; i < this->l; i++){
    
        /* Find position in table */
        pos = this->hashFunctions[i]->hash(query,status);
        if(status != 0)
            return;

        /* Empty list */
        if(this->tables[i][pos].size() == 0)
            continue;

        /* Find value g for query */
        valueG = this->hashFunctions[i]->hashLevel2(query,status);
        if(status != 0)
            return;

        /* Scan list of specific bucket */
        for(iter = this->tables[i][pos].begin(); iter != this->tables[i][pos].end(); iter++){  

            /* Compare values g of query and current point */
            if(valueG != iter->valueG)
                continue;

            /* Find current distance */
            currDist = iter->point.euclideanDist(query,status);
            if(status != 0)
                return;
            
            /* First neighbor */
            if(flag == 0){
                minDist = currDist;
                iterNearestNeighbor = iter;
                
                found = 1;
                flag = 1;
            }

            /* Change min distance */
            else if(minDist > currDist){
                minDist = currDist;
                iterNearestNeighbor = iter;
            }

        } // End for - Scan list
    } // End for - Tables

    /* Nearest neighbor found */
    if(found == 1){
        nNeighbor = iterNearestNeighbor->point;
        if(neighborDistance != NULL)
            *neighborDistance = minDist;
    }
    else{
        nNeighbor.setId("Nearest neighbor not found");
        if(neighborDistance != NULL)
            *neighborDistance = minDist;
    }
}


///////////////
/* Accessors */
///////////////

int lshEuclidean::getNumberOfPoints(errorCode& status){
    status = SUCCESS;

    if(fitted == 0){
        status = METHOD_UNFITTED;
        return 0;
    }
    else
        return this->n;
}

int lshEuclidean::getDim(errorCode& status){
    status = SUCCESS;

    if(fitted == 0){
        status = METHOD_UNFITTED;
        return 0;
    }
    else
        return this->dim;
}


/* Print statistics */
void lshEuclidean::print(void){

    if(this->k == -1)
        cout << "Invalid method\n";
    else{

        cout << "Lsh euclidean statistics\n";
        cout << "Number of hash tables(l): " << this->l << "\n";
        cout << "Size per table: " << this->tableSize << "\n";
        cout << "Coefficient factor: " << this->coefficient << "\n";
        cout << "Window size(w): " << this->w << "\n";
        cout << "Number of sub hash functions(k): " << this->k << "\n\n";
    }
}

void lshEuclidean::printHashFunctions(void){

    if(this->k == -1)
        cout << "Invalid method\n";
    else if(this->fitted == 0)
        cout << "Method not fitted\n";
    else{

        int i;
        for(i = 0; i < this->l; i++)
            this->hashFunctions[i]->print();
    }
}


///////////////////////////////////////
/* Implementation of lsh cosin class */
///////////////////////////////////////
