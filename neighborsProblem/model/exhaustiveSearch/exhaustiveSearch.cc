#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include "exhaustiveSearch.h"
#include "../../hashFunction/hashFunction.h"
#include "../../item/item.h"
#include "../../utils/utils.h"

using namespace std;

/////////////////////////////////////////
/* Implementation of exhaustive search */
/////////////////////////////////////////

/* Default constructor */
exhaustiveSearch::exhaustiveSearch():tableSize(0),n(0),dim(0),fitted(0){}

exhaustiveSearch::~exhaustiveSearch(){}

/* Save given points */
void exhaustiveSearch::fit(list<Item>& points, errorCode& status){
    
    /* Iteratiors */
    list<Item>::iterator iterPoints = points.begin(); // Iterate through points
   
    status = SUCCESS;

    /* Already fitted */
    if(this->fitted == 1){
        status = METHOD_ALREADY_USED;
        return;
    }

    /* Check parameters */
    this->points.reserve(points.size());

    /* Set members */
    this->n = points.size();
    if(this->n < MIN_POINTS || this->n > MAX_POINTS){
        status = INVALID_POINTS;
        return;
    }
 
    this->tableSize = this->n;
    /* Copy points */
    for(iterPoints = points.begin(); iterPoints != points.end(); iterPoints++)
        this->points.push_back(*iterPoints);

    this->fitted = 1;
}

/* Find the radius neighbors of a given point */
void exhaustiveSearch::radiusNeighbors(Item& query, int radius, list<Item>& neighbors, list<double>* neighborsDistances, errorCode& status){
    int i;
    double currDist; // Distance of a point in list

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

    /* Clear given lists */
    neighbors.clear();
    if(neighborsDistances != NULL)
        neighborsDistances->clear();

    /* Scann all points */
    for(i = 0; i < this->tableSize; i++){
        /* Find current distance */
        currDist = this->points[i].euclideanDist(query, status);
        if(status != SUCCESS)
            return;

        /* Keep neighbor */
        if(currDist < radius){
            neighbors.push_back(this->points[i]);
            if(neighborsDistances != NULL)
                neighborsDistances->push_back(currDist);
        }
    } // End for
}

/* Find the nearest neighbor of a given point */
void exhaustiveSearch::nNeighbor(Item& query, Item& nNeighbor, double* neighborDistance, errorCode& status){
    int i, posMin = 0;
    double minDist = -1; // Current minimum distance 
    double currDist; // Distance of a point in list

    status = SUCCESS;

    /* Check model */
    if(this->fitted == 0){
        status = METHOD_UNFITTED;
        return;
    }

    /* Scann all points */
    for(i = 0; i < this->tableSize; i++){
        
        /* Get current distance from query */
        currDist = this->points[i].euclideanDist(query, status);
        if(status != SUCCESS)
            return;
    
        if(i == 0){
            posMin = i;
            minDist = currDist;
        }
        else if(minDist > currDist){
            posMin = i;
            minDist = currDist;
            
        }
    } // End for

    /* Set nearest neighbor */
    nNeighbor = this->points[posMin];
    if(neighborDistance != NULL)
        *neighborDistance = minDist;
}


///////////////
/* Accessors */
///////////////

int exhaustiveSearch::getNumberOfPoints(errorCode& status){
    status = SUCCESS;

    if(fitted == 0){
        status = METHOD_UNFITTED;
        return -1;
    }
    else
        return this->n;
}

int exhaustiveSearch::getDim(errorCode& status){
    status = SUCCESS;

    if(fitted == 0){
        status = METHOD_UNFITTED;
        return -1;
    }
    else
        return this->dim;
}

unsigned exhaustiveSearch::size(void){
    unsigned result = 0;

    if(this->fitted == 0)
        return -1;
    
    result += sizeof(this->tableSize);
    result += sizeof(this->n);
    result += sizeof(this->dim);
    result += sizeof(this->fitted);
    
    int i;

    for(i = 0; i < this->tableSize; i++)
        result += this->points.size();

    result += this->points.capacity() * sizeof(Item);
        
    return result;
}

/* Print statistics */
void exhaustiveSearch::print(void){

    if(this->fitted == 0)
        cout << "Method unfitted\n";
    else{

        cout << "Exhaustive search statistics\n";
        cout << "Dimension: " << this->dim << "\n";
        cout << "Total points: " << this->n << "\n\n";
    }
}

void exhaustiveSearch::printHashFunctions(void){
    cout << "Exhaustive search hans't hash fucntions\n\n";
}

// Petropoulakis Panagiotis
