#include <iostream>
#include <string>
#include <new>
#include "item.h"
#include "myLimits.h"

using namespace std;

////////////////////////////////
/* Constructors - destructors */
////////////////////////////////

Item::Item(){}

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
Item::Item(string& id, vector<double>& components, int&status):{
    status = 0;

    if(components.size() <= 0 || components.size() >MAX_DIM)
        status = -2;
    else{
        this->id = id;
        this->components = components;
        this->dim = components.size();
    }
}

/* Copy constructor */
Item::Item(const Item copy):id(copy.id),components(copy.components),dim(copy.dim){}

Item::~Item(){}

/* Overide assignment operator */
Item& Item::operator=(const Item& x){

    if(this == &x)
        return *this;

    this->id = x.id;
    this->components = x.components;
    this->dim = dim;

    return *this;
}

//////////////
/* Mutators */
//////////////

/* Set id of item */
void Item::ItemSetId(string& newId){
    this->id = newId;
}

/* Set component-i */
void Item::ItemSetComponent(double& newComponent, int& index, int& status){    
    ststus = 0;

    /* Check parameters */
    if(index < 0 || index >= this->dim)
        status =  -1;
    else
        this->components[index] = newComponent;
}

/* Append new component */
void Item::ItemAppendComponent(double& newComponent, int& status){
    status = 0;

    /* Check size of components */
    if(this->dim == MAX_DIM)
        status = -2;
    else{
        this->components.append(newComponent);
        this->dim += 1;
    }
}

///////////////
/* Accessors */
///////////////

string Item::ItemGetId(void){
    return this->id;
}

/* Get component-i */
double Item::ItemGetComponent(int& index, int& status){
    status = 0;

    /* Check parameters */
    if(index < 0 || index >= this->dim){
        error =  -1;
        return 0;
    }
    else
        return this->components[index];
}

/* Print Item */
void Item::ItemPrint(void){
    int i;

    cout <<  "Item id: " << this->id << "\n";
    cout <<  "Item components: ";
    
    for(i = 0; i < this->dim; i++)
        cout << this->components[i] << " ";

    cout << "\n";
}

/* Calculate inner product of two items */
friend double Item::innerProduct(Item& x, Item& y, int& status){
    double product = 0;
    int i;

    status = 0;

    /* Check dimensions */
    if(x.dim == 0 || y.dim == 0){
        error = -2;
        return 0;
    }

    if(x.dim != y.dim){
        error = -2;
        return 0;
    }

    /* Calculate product */
    for(i = 0; i < dim; i++)
        product += x.components[i] * y.components[i];
    
    return product;
}

//////////////
/* Metrices */
//////////////

friend double Item::euclideanDist(Item& x, Item& y, int& status){
    double dist = 0;
    int i;
    double newComponent;

    status = 0;

    /* Check dimensions */
    if(x.dim == 0 || y.dim == 0){
        error = -2;
        return 0;
    }

    if(x.dim != y.dim){
        error = -2;
        return 0;
    }

    /* Calculate distance */
    for(i = 0; i < dim; i++){
        newComponent = x.components[i] - y.components[i];
        dist += newComponent * newComponent; 
    } // End for
    
    dist = sqrt(dist);

    return dist;
}

friend double Item::cosinDist(Item& x, Item& y, int& status){
    double dist = 0;
    int i;
    double newComponent;

    status = 0;

    /* Check dimensions */
    if(x.dim == 0 || y.dim == 0){
        error = -2;
        return 0;
    }

    if(x.dim != y.dim){
        error = -2;
        return 0;
    }

    /* Calculate distance */
    for(i = 0; i < dim; i++){
        newComponent = x.components[i] - y.components[i];
        dist += newComponent * newComponent; 
    } // End for
    
    dist = sqrt(dist);

    return dist;
}

// PetropoulakisPanagiotis
