#include <iostream>
#include <vector>
#include <unordered_set>
#include <list>
#include <cmath>
#include "lsh.h"
#include "../../hashFunction/hashFunction.h"
#include "../../item/item.h"
#include "../../utils/utils.h"

using namespace std;

///////////////////////////////////////
/* Implementation of lsh cosin class */
///////////////////////////////////////

/* Default constructor */
lshCosin::lshCosin():tableSize(0),n(0),l(5),k(4),dim(0),fitted(0){
    int i;
    
    /* Set size of hash functions */
    this->hashFunctions.reserve(this->l);
    for(i = 0; i < this->l; i++)
        this->hashFunctions[i] = NULL;

    /* Set size of hash tables */
    for(i = 0; i < this->l; i++)
        this->tables.push_back(vector<list<Item> >(this->l));
}

lshCosin::lshCosin(int k, int l, errorCode& status):tableSize(0),n(0),l(l),k(k),dim(0),fitted(0){
    int i;
    
    /* Check parameters */
    if(l < MIN_L || l > MAX_L || k < MIN_K || k > MAX_K){
        status = INVALID_PARAMETERS;
        this->k = -1;
    }
    else{

        /* Set size of hash functions */
        this->hashFunctions.reserve(this->l);
        for(i = 0; i < this->l; i++)
            this->hashFunctions[i] = NULL;

        /* Set size of hash tables */
        for(i = 0; i < this->l; i++)
            this->tables.push_back(vector<list<Item> >(this->l));
    }
}

lshCosin::~lshCosin(){
 
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

/* Fix hash table, members of lsh cosin and add given points in the hash tables */
void lshCosin::fit(list<Item>& points, errorCode& status){
    int i, j;
    int pos; // Pos(line) in current hash table

    /* Iteratiors */
    list<Item>::iterator iterPoints = points.begin(); // Iterate through points
    list<Item>::iterator iterTables;  // Iterate through table
   
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

    /* Set members */
    this->n = points.size();
    if(this->n < MIN_POINTS || this->n > MAX_POINTS){
        status = INVALID_POINTS;
        return;
    }

    /* Set table size */
    this->tableSize = pow(2,this->k);

    /* Fix each table - Each table contains lists of entries */
    for(i = 0; i < this->l; i++)
        for(j = 0; j < this->tableSize; j++)
            this->tables[i].push_back(list<Item>());

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
    hashFunctionCosin* newFunc = NULL;  
   
    for(i = 0; i < this->l; i++){
        newFunc = new hashFunctionCosin(this->dim, this->k);
       
        /* Truncate same hash functions */
        for(j = 0; j < i; j++){
            if(this->hashFunctions[j]->compare(*newFunc, status) == 0){
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

        if(i == j){
            this->hashFunctions[i] = newFunc; // Add hash function
        }
        else
            i -= 1;
    } // End for - Hash functions

    /* Delete remaining hash functions */
    if(status == ALLOCATION_FAILED){
        for(j = 0; j < i; j++)
            delete this->hashFunctions[j];
        return;
    }

    /////////////////////
    /* Set hash tables */
    /////////////////////

    /* Scan each table */
    for(i = 0; i < this->l; i++){
    
        /* Scan given points */
        for(iterPoints = points.begin(); iterPoints != points.end(); iterPoints++){
            /* Check consistency of dim */
            if(this->dim != iterPoints->getDim()){
                status = INVALID_DIM;
                break;
            }
            
            /* Find position in hash table */
            pos = this->hashFunctions[i]->hash(*iterPoints, status);
           
            if(pos < 0 || pos >= tableSize){
                status = INVALID_HASH_FUNCTION;
                break;
            }
            
            if(status != SUCCESS){
                this->k = -1;
                break;
            }

            /* Add point */
            this->tables[i][pos].push_back(*(iterPoints));
        } // End for - Points

        if(status != SUCCESS)
            break;
    } // End for - Hash tables
  
    /* Error occured - Clear structures */
    if(status != SUCCESS){
       
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
void lshCosin::radiusNeighbors(Item& query, int radius, list<Item>& neighbors, list<double>* neighborsDistances, errorCode& status){
    int queryDim = query.getDim();
    int i, pos;
    double currDist; // Distance of a point in list
    list<Item>::iterator iter;
    unordered_set<string> visited; // Visited points
    string currId;

    status = SUCCESS;

    /* Check parameters */
    if(radius < MIN_RADIUS || radius > MAX_RADIUS){
        status = INVALID_RADIUS;
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

    /* Clear given lists */
    neighbors.clear();
    if(neighborsDistances != NULL)
        neighborsDistances->clear();

    /* Scan all tables */
    for(i = 0; i < this->l; i++){
    
        /* Find position in table */
        pos = this->hashFunctions[i]->hash(query, status);
        if(status != SUCCESS)
            return;

        /* Empty list */
        if(this->tables[i][pos].size() == 0)
            continue;

        /* Scan list of specific bucket */
        for(iter = this->tables[i][pos].begin(); iter != this->tables[i][pos].end(); iter++){  

            /* Check if current points is checked */
            currId = iter->getId();

            /* Not found - add it */
            if(visited.find(currId) == visited.end()){
                visited.insert(currId);
            }
            /* Vidited - Discard it */
            else
                continue;
            
            /* Find current distance */
            currDist = iter->cosinDist(query, status);
            if(status != SUCCESS)
                return;
            
            /* Keep neighbor */
            if(currDist < radius){
                neighbors.push_back(*(iter));
                if(neighborsDistances != NULL)
                    neighborsDistances->push_back(currDist);
            }
        } // End for - Scan list
    } // End for - Tables
}

/* Find the nearest neighbor of a given point */
void lshCosin::nNeighbor(Item& query, Item& nNeighbor, double* neighborDistance, errorCode& status){
    int queryDim = query.getDim();
    int i, pos, found = 0, flag = 0;
    double minDist = -1; // Current minimum distance 
    double currDist; // Distance of a point in list
    list<Item>::iterator iter;
    list<Item>::iterator iterNearestNeighbor;
    unordered_set<string> visited; // Visited points
    string currId;

    status = SUCCESS;

    /* Check parameters */
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
        pos = this->hashFunctions[i]->hash(query, status);
        if(status != SUCCESS)
            return;

        /* Empty list */
        if(this->tables[i][pos].size() == 0)
            continue;

        /* Scan list of specific bucket */
        for(iter = this->tables[i][pos].begin(); iter != this->tables[i][pos].end(); iter++){  

            /* Check if current points is checked */
            currId = iter->getId();

            /* Not found - add it */
            if(visited.find(currId) == visited.end()){
                visited.insert(currId);
            }
            /* Vidited - Discard it */
            else
                continue;
            
            /* Find current distance */
            currDist = iter->cosinDist(query, status);
            if(status != SUCCESS)
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
        nNeighbor = *(iterNearestNeighbor);
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

int lshCosin::getNumberOfPoints(errorCode& status){
    status = SUCCESS;

    if(fitted == 0){
        status = METHOD_UNFITTED;
        return -1;
    }
    else if(this->k == -1){
        status = INVALID_METHOD;
        return -1;
    }
    else
        return this->n;
}

int lshCosin::getDim(errorCode& status){
    status = SUCCESS;

    if(fitted == 0){
        status = METHOD_UNFITTED;
        return -1;
    }
    else if(this->k == -1){
        status = INVALID_METHOD;
        return -1;
    }
    else
        return this->dim;
}

unsigned lshCosin::size(void){
    return sizeof(*this);
}

/* Print statistics */
void lshCosin::print(void){

    if(this->k == -1)
        cout << "Invalid method\n";
    else{

        cout << "Lsh cosin statistics\n";
        cout << "Number of hash tables(l): " << this->l << "\n";
        cout << "Size per table: " << this->tableSize << "\n";
        cout << "Number of sub hash functions(k): " << this->k << "\n\n";
    }
}

void lshCosin::printHashFunctions(void){

    if(this->k == -1)
        cout << "Invalid method\n";
    else if(this->fitted == 0)
        cout << "Method not fitted\n";
    else{

        int i;

        cout << "Hash functions of lsh cosin\n";
        for(i = 0; i < this->l; i++)
            this->hashFunctions[i]->print();
    }
}


