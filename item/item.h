#pragma once
#include <string>
#include <vector>
#include "../utils/utils.h"

/* Item class represents the points in data sets */
/* Each point is a vector with a given name      */
class Item{
    private:
        std::string id;
        std::vector<float> components;
        int dim; // Dimension
        static int count;

    public:
        Item();
        Item(int,errorCode&);
        Item(std::string,std::vector<float>&,errorCode&);

        ~Item();

        /* Mutators */
        void setId(std::string);
        void setComponent(float,int,errorCode&);
        void appendComponent(float,errorCode&);
        void concatenateComponents(std::vector<float>&,errorCode&);
        void resetComponents(std::vector<float>&,errorCode&);

        /* Accessors */
        std::string getId(void);
        float getComponent(int,errorCode&);
        int getCount(void);
        void print(void);

        /* Usefull functions */
        float innerProduct(Item&,errorCode&); 
        float norm(errorCode&);
        
        /* Metrices */
        float euclideanDist(Item&,errorCode&);
        float cosinDist(Item&,errorCode&);
};
// PetropoulakisPanagiotis
