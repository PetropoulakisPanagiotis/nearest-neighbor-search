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
    this->id = "item_" + count;
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
        this->id = "item_" + count;

        this->count += 1;
        this->components.reserve(dim);
        this->dim = dim;
    }
}

/* Set all members */
Item::Item(string id, vector<float>& components, errorCode& status){
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
Item::Item(vector<float>& components, errorCode& status){
    status = SUCCESS;

    if(components.size() <= 0 || components.size() > MAX_DIM)
        status = INVALID_DIM;
    else{
        
        this->id = "item_" + count;
        this->components = components;
        this->dim = components.size();
        
        this->count += 1;
    }
}

Item::~Item(){}

//////////////
/* Mutators */
//////////////

/* Set id of item */
void Item::setId(string newId){
    this->id = newId;
}

/* Set component-i */
void Item::setComponent(float newComponent, int index, errorCode& status){    
    status = SUCCESS;

    /* Check parameters */
    if(index < 0 || index >= this->dim)
        status =  INVALID_INDEX;
    else
        this->components[index] = newComponent;
}

/* Append new component */
void Item::appendComponent(float newComponent, errorCode& status){
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
void Item::concatenateComponents(vector<float>& newComponents, errorCode& status){
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
void Item::resetComponents(vector<float>& newComponents, errorCode& status){    
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
float Item::getComponent(int index, errorCode& status){
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

/* Calculate inner product of two items */
float Item::innerProduct(Item& x, errorCode& status){
    float product = 0;
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
    for(i = 0; i < this->dim; i++)
        product += this->components[i] * x.components[i];
    
    return product;
}

/* Get norm of vector */
float Item::norm(errorCode& status){
    float norm = 0;
    int i;

    status = SUCCESS;

    /* Check dimensions */
    if(this->dim == 0){
        status = INVALID_DIM;
        return 0;
    }

    /* Calculate norm */
    for(i = 0; i < this->dim; i++)
        norm += this->components[i] * this->components[i]; 
    
    norm = sqrt(norm);

    return norm;
}

//////////////
/* Metrices */
//////////////

float Item::euclideanDist(Item& x, errorCode& status){
    float dist = 0, newComponent;
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
        newComponent = this->components[i] - x.components[i];
        dist += newComponent * newComponent; 
    } // End for
    
    dist = sqrt(dist);

    return dist;
}

float Item::cosinDist(Item& x, errorCode& status){
    float dist = 0, mult;
    int i;

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

    mult = this->norm(status);
    if(status != SUCCESS)
        return 0;

    mult *= x.norm(status);
    if(status != SUCCESS)
        return 0;

    dist /= mult;
    
    dist = 1 - dist;

    return dist;
}
// PetropoulakisPanagiotis
