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

        ~Item();

        /* Mutators */
        void setId(std::string&);
        void setComponent(double&,int&,int&);
        void appendComponent(double&,int&);
        void concatenateComponents(std::vector<double>&,int&);
        void resetComponents(std::vector<double>&,int&);

        /* Accessors */
        std::string getId(void);
        double getComponent(int&,int&);
        void print(void);

        /* Usefull functions */
        double innerProduct(Item&,int&); 
        double norm(int&);
        
        /* Metrices */
        double euclideanDist(Item&,int&);
        double cosinDist(Item&,int&);
};
// PetropoulakisPanagiotis
