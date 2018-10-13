#pragma once
#include <string>
#include <vector>

/* Item class represents the points in data sets */
/* Each point is a vector with a given name      */
/* Error codes:                                  */
/* 0: Success                                    */
/* -1: Out of range index/empty item             */
/* -2: Inconsistent dimensions                   */
class Item{
    private:
        std::string id;
        std::vector<double> components;
        int dim; // Dimension

    public:
        Item();
        Item(int&,int&);
        Item(std::string&,std::vector<double>&,int&);
        Item(const Item&);

        ~Item();

        /* Override operators */
        Item& operator=(const Item&);
        
        /* Mutators */
        void ItemSetId(std::string&);
        void ItemSetComponent(double&,int&,int&);
        void ItemAppendComponent(double&,int&);

        /* Accessors */
        std::string ItemGetId(void);
        double ItemGetComponent(int&,int&);
        void ItemPrint(void);

        /* Usefull functions */
        friend double innerProduct(Item&,Item&,int&); 
        friend double norm(Item&,int&);
        
        /* Metrices */
        friend double euclideanDist(Item&,Item&,int&);
        friend double cosinDist(Item&,Item&,int&);
};
// PetropoulakisPanagiotis
