#include <iostream>
#include <string>
#include <cmath>
#include "item.h"
#include "../myLimits/myLimits.h"

using namespace std;

////////////////////////////////
/* Constructors - destructors */
////////////////////////////////

Item::Item():dim(0){}

/* Set dimensions of item */
Item::Item(int& dim, int& status){
    status = 0;

    if(dim <= 0 || dim > MAX_DIM)
        status = -2;
    else{ 
        this->components.reserve(dim);
        this->dim = dim;
    }
}

/* Set all members */
Item::Item(string& id, vector<double>& components, int&status){
    status = 0;

    if(components.size() <= 0 || components.size() >MAX_DIM)
        status = -2;
    else{
        this->id = id;
        this->components = components;
        this->dim = components.size();
    }
}

Item::~Item(){}

//////////////
/* Mutators */
//////////////

/* Set id of item */
void Item::setId(string& newId){
    this->id = newId;
}

/* Set component-i */
void Item::setComponent(double& newComponent, int& index, int& status){    
    status = 0;

    /* Check parameters */
    if(index < 0 || index >= this->dim)
        status =  -1;
    else
        this->components[index] = newComponent;
}

/* Append new component */
void Item::appendComponent(double& newComponent, int& status){
    status = 0;

    /* Check size of components */
    if(this->dim == MAX_DIM)
        status = -2;
    else{
        this->components.push_back(newComponent);
        this->dim += 1;
    }
}

/* Concatenate given components with current components */
void Item::concatenateComponents(vector<double>& newComponents, int& status){
    status = 0;

    /* Check size of components */
    if(this->dim + newComponents.size() >= MAX_DIM)
        status = -2;
    else if(newComponents.size() != 0){
        this->components.insert(this->components.end(),newComponents.begin(),newComponents.end());
        this->dim = this->components.size();
    }
}

/* Reset all components. Copy given values in current item */
void Item::resetComponents(vector<double>& newComponents, int& status){    
    status = 0;

    /* Check parameters */
    if(newComponents.size() >= MAX_DIM)
        status =  -2;
    else if(newComponents.size() != 0){
        this->components.assign(newComponents.begin(),newComponents.end());
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
double Item::getComponent(int& index, int& status){
    status = 0;

    /* Check parameters */
    if(index < 0 || index >= this->dim){
        status =  -1;
        return 0;
    }
    else
        return this->components[index];
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
double Item::innerProduct(Item& x, int& status){
    double product = 0;
    int i;

    status = 0;

    /* Check dimensions */
    if(this->dim == 0 || x.dim == 0){
        status = -2;
        return 0;
    }

    if(this->dim != x.dim){
        status = -2;
        return 0;
    }

    /* Calculate product */
    for(i = 0; i < this->dim; i++)
        product += this->components[i] * x.components[i];
    
    return product;
}

double Item::norm(int& status){
    double norm = 0;
    int i;
    double newComponent;

    status = 0;

    /* Check dimensions */
    if(this->dim == 0){
        status = -2;
        return 0;
    }

    /* Calculate norm */
    for(i = 0; i < this->dim; i++){
        newComponent = this->components[i];
        norm += newComponent * newComponent; 
    } // End for
    
    norm = sqrt(norm);

    return norm;
}

//////////////
/* Metrices */
//////////////

double Item::euclideanDist(Item& x, int& status){
    double dist = 0;
    int i;
    double newComponent;

    status = 0;

    /* Check dimensions */
    if(this->dim == 0 || x.dim == 0){
        status = -2;
        return 0;
    }

    if(this->dim != x.dim){
        status = -2;
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

double Item::cosinDist(Item& x, int& status){
    double dist = 0;
    int i;
    double mult;

    status = 0;

    /* Check dimensions */
    if(this->dim == 0 || x.dim == 0){
        status = -2;
        return 0;
    }

    if(this->dim != x.dim){
        status = -2;
        return 0;
    }

    dist = this->innerProduct(x,status);
    if(status != 0)
        return 0;

    mult = this->norm(status);
    if(status != 0)
        return 0;

    mult *= x.norm(status);
    if(status != 0)
        return 0;

    dist /= mult;
    
    dist = 1 - dist;

    return dist;
}
// PetropoulakisPanagiotis
