#include <iostream>
#include <string>
#include <cmath>
#include "item.h"
#include "../utils/myLimits.h"
#include "../utils/utils.h"

using namespace std;

/* Initialize static fields */
int Item:: count = 0;

////////////////////////////////
/* Constructors - destructors */
////////////////////////////////

Item::Item(){
    this->id = "item_" + to_string(count);
    this->count += 1;

    this->dim = 0;
}

/* Set dimensions of item */
Item::Item(string id, int dim, errorCode& status){
    status = SUCCESS;

    if(dim <= 0 || dim > MAX_DIM)
        status = INVALID_DIM;
    else{ 
        this->id = id;

        this->count += 1;
        this->components.reserve(dim);
        this->dim = dim;
    }
}

/* Set dimensions of item */
Item::Item(int dim, errorCode& status){
    status = SUCCESS;

    if(dim <= 0 || dim > MAX_DIM)
        status = INVALID_DIM;
    else{ 
        this->id = "item_" + to_string(count);

        this->count += 1;
        this->components.reserve(dim);
        this->dim = dim;
    }
}

/* Set all members */
Item::Item(string id, vector<double>& components, errorCode& status){
    status = SUCCESS;

    if(components.size() <= 0 || components.size() > MAX_DIM)
        status = INVALID_DIM;
    else{
        this->id = id;
        this->components = components;
        this->dim = components.size();
        
        this->count += 1;
    }
}

/* Set all members */
Item::Item(vector<double>& components, errorCode& status){
    status = SUCCESS;

    if(components.size() <= 0 || components.size() > MAX_DIM)
        status = INVALID_DIM;
    else{
        
        this->id = "item_" + to_string(count);
        this->components = components;
        this->dim = components.size();
        
        this->count += 1;
    }
}

Item::~Item(){
    this->count -= 1;
}

//////////////
/* Mutators */
//////////////

/* Set id of item */
void Item::setId(string newId){
    this->id = newId;
}

/* Set component-i */
void Item::setComponent(double newComponent, int index, errorCode& status){    
    status = SUCCESS;

    /* Check parameters */
    if(index < 0 || index >= this->dim)
        status =  INVALID_INDEX;
    else
        this->components[index] = newComponent;
}

/* Append new component */
void Item::appendComponent(double newComponent, errorCode& status){
    status = SUCCESS;

    /* Check size of components */
    if(this->dim == MAX_DIM)
        status = INVALID_DIM;
    else{
        this->components.push_back(newComponent);
        this->dim += 1;
    }
}

/* Concatenate given components with current components */
void Item::concatenateComponents(vector<double>& newComponents, errorCode& status){
    status = SUCCESS;

    /* Check size of components */
    if(this->dim + newComponents.size() >= MAX_DIM)
        status = INVALID_DIM;
    else if(newComponents.size() != 0){
        this->components.insert(this->components.end(), newComponents.begin(), newComponents.end());
        this->dim = this->components.size();
    }
}

/* Reset all components. Copy given values in current item */
void Item::resetComponents(vector<double>& newComponents, errorCode& status){    
    status = SUCCESS;

    /* Check parameters */
    if(newComponents.size() >= MAX_DIM)
        status =  INVALID_DIM;
    else if(newComponents.size() != 0){
        this->components.assign(newComponents.begin(), newComponents.end());
        this->dim = this->components.size();
    }
}

///////////////
/* Accessors */
///////////////

string Item::getId(void){
    return this->id;
}

/* Get component-i */
double Item::getComponent(int index, errorCode& status){
    status = SUCCESS;

    /* Check parameters */
    if(index < 0 || index >= this->dim){
        status =  INVALID_INDEX;
        return 0;
    }
    else
        return this->components[index];
}

/* Get total items */
int Item::getCount(void){
    return this->count;
}

/* Print Item */
void Item::print(void){
    int i;

    cout <<  "Item id: " << this->id << "\n";
    cout <<  "Item dimension: " << this->dim << "\n";
    cout <<  "Item components: ";
    
    for(i = 0; i < this->dim; i++)
        cout << this->components[i] << " ";

    cout << "\n\n";
}

////////////////////////
/* Usefull operations */
////////////////////////

/* Compare given items */
/* Discard id          */
/* Equal: 0            */
/* Note equal: 1       */
int Item::compare(Item& x, errorCode& status){

    status = SUCCESS;

    /* Check dimensions */
    if(this->dim == 0){
        status = INVALID_DIM;
        return 0;
    }
    
    if(this->dim != x.dim)
        return 1;

    if(equal(this->components.begin(), this->components.end(),x.components.begin()))
        return 0;
    else
        return 1;
}

/* Calculate inner product of two items */
double Item::innerProduct(Item& x, errorCode& status){
    double product = 0, tempMult;
    int i;

    status = SUCCESS;

    /* Check dimensions */
    if(this->dim == 0 || x.dim == 0){
        status = INVALID_DIM;
        return 0;
    }

    if(this->dim != x.dim){
        status = INVALID_DIM;
        return 0;
    }

    /* Calculate product */
    for(i = 0; i < this->dim; i++){
        tempMult= myMult(this->components[i],x.components[i],status);
        if(status != SUCCESS)
            return 0;

        product = mySum(tempMult,product,status);
        if(status != SUCCESS)
            return 0;
    } // End for

    return product;
}

/* Get norm of vector */
double Item::norm(errorCode& status){
    double norm = 0, tempMult;
    int i;

    status = SUCCESS;

    /* Check dimensions */
    if(this->dim == 0){
        status = INVALID_DIM;
        return 0;
    }

    /* Calculate norm */
    for(i = 0; i < this->dim; i++){
        tempMult= myMult(this->components[i],this->components[i],status);
        if(status != SUCCESS)
            return 0;

        norm = mySum(tempMult,norm,status);
        if(status != SUCCESS)
            return 0;
    } // End for

    norm = sqrt(norm);

    return norm;
}

//////////////
/* Metrices */
//////////////

double Item::euclideanDist(Item& x, errorCode& status){
    double dist = 0, newComponent, tempMult;
    int i;

    status = SUCCESS;

    /* Check dimensions */
    if(this->dim == 0 || x.dim == 0){
        status = INVALID_DIM;
        return 0;
    }

    if(this->dim != x.dim){
        status = INVALID_DIM;
        return 0;
    }

    /* Calculate distance */
    for(i = 0; i < this->dim; i++){
        newComponent = mySub(this->components[i],x.components[i],status);
        if(status != SUCCESS)
            return 0;

        tempMult= myMult(newComponent,newComponent,status);
        if(status != SUCCESS)
            return 0;

        dist = mySum(dist,tempMult,status);
        if(status != SUCCESS)
            return 0;
    } // End for
    
    dist = sqrt(dist);

    return dist;
}

/* dist(x,y) = 1 cos(x,y) = 1 - (x.y / norm(x) * norm(y)) */
double Item::cosinDist(Item& x, errorCode& status){
    double dist = 0, mult;
    int i;
    double normX, normY;

    status = SUCCESS;

    /* Check dimensions */
    if(this->dim <= 0 || x.dim <= 0){
        status = INVALID_DIM;
        return 0;
    }

    if(this->dim != x.dim){
        status = INVALID_DIM;
        return 0;
    }

    dist = this->innerProduct(x,status);
    if(status != SUCCESS)
        return 0;

    normX = this->norm(status);
    if(status != SUCCESS)
        return 0;

    normY = x.norm(status);
    if(status != SUCCESS)
        return 0;

    mult = myMult(normX,normY,status);
    if(status != SUCCESS)
        return 0;

    dist = myDiv(dist,mult,status);
    if(status != SUCCESS)
        return 0;

    dist = mySub(1,dist,status);
    if(status != SUCCESS)
        return 0;

    return dist;
}
// PetropoulakisPanagiotis
