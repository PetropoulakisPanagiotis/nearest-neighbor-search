#pragma once
#include <string>
#include <vector>
#include "../utils/utils.h"

/* Item class represents the points in data sets */
/* Each point is a vector with a given name      */
class Item{
    private:
        std::string id;
        std::vector<double> components;
        int dim; // Dimension
        static int count;

    public:
        Item();
        Item(int dim,errorCode&);
        Item(std::string,int dim,errorCode&);
        Item(std::vector<double>&,errorCode&);
        Item(std::string,std::vector<double>&,errorCode&);

        ~Item();

        /* Mutators */
        void setId(std::string);
        void setComponent(double,int,errorCode&);
        void appendComponent(double,errorCode&);
        void concatenateComponents(std::vector<double>&,errorCode&);
        void resetComponents(std::vector<double>&,errorCode&);

        /* Accessors */
        std::string getId(void);
        double getComponent(int,errorCode&);
        int getCount(void);
        void print(void);

        /* Usefull functions */
        double innerProduct(Item&,errorCode&); 
        double norm(errorCode&);
        
        /* Metrices */
        double euclideanDist(Item&,errorCode&);
        double cosinDist(Item&,errorCode&);
};
// PetropoulakisPanagiotis
