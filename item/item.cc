#include <iostream>
#include <string>
#include <new>
#include "item.h"

using namespace std;

/* Construct a new item */
Item::Item(){}
Item::Item(string& id, vector<double>& components):id(id),components(components){}
Item::~Item(){}

// Mutators //

// Set id of item //
void Item::ItemSetId(string& newId){
    this->id = newId;
}

// Set component i of item //
void Item::ItemSetComponent(double& newComponent, int& index, int& error){    
    error = 0;

    // Check parameters //
    if(index < 0 || index >= this->components.size())
        error =  -1;
    else
        this->components[index] = newComponent;
}

// Accessors //

string Item::ItemGetId(void){
    return this->id;
}

double Item::ItemGetComponent(int& index, int& error){
    error = 0;

    // Check parameters //
    if(index < 0 || index >= this->components.size()){
        error =  -1;
        return 0;
    }
    else
        return this->components[index];
}

void Item::ItemPrint(void){
    int i;

    cout <<  "Item id: " << this->id << "\n";
    cout <<  "Item components: " << this->id;
    
    for(i = 0; i < this->components.size(); i++)
        cout << this->components[i];

    cout << "\n";
}

// Metrices //
friend double Item::EuclideanDist(Item& x, Item& y, int& error){

    // Check dimensions //
    if(x.components.size() == 0 || y.components.size() == 0){
        error = -1;
        return 0;
    }
    if(x.components.size() != y.components.size()){
        error = -2;
        return 0;
    }

    double dist = 0;
    int i;

    for(i = 0; i < x.components.size(); i++)
        dist += (x.components[i] - x.components[i]) * 

    dist = sqrt(dist);

    return dist;
}

// PetropoulakisPanagiotis
