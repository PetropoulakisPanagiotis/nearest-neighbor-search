#pragma once
#include <string>
#include <vector>

// Item class represents the points in data sets //
// Each point is a vector with a given name      //
// Error codes                                   //
// 0: Success                                    //
// -1: Out of range index/empty item             //
// -2: Inconsistent dimensions                   //
class Item{
    private:
        std::string id;
        std::vector<double> components;
    
    public:
        Item();
        Item(std::string&,std::vector<double>&);
        ~Item();

        // Mutators //
        void ItemSetId(std::string&);
        void ItemSetComponent(double&,int&,int&);
    
        // Accessors //
        std::string ItemGetId(void);
        double ItemGetComponent(int&,int&);
        void ItemPrint(void);


        // Metrices //
        friend double EuclideanDist(Item&,Item&,int&);
        friend double CosinDist(Item&,Item&,int&);
};

// PetropoulakisPanagiotis
