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
        Item(int dim, errorCode& status);
        Item(std::string id, int dim, errorCode& status);
        Item(std::vector<double>& components, errorCode& status);
        Item(std::string id, std::vector<double>& components, errorCode& status);

        ~Item();

        /* Mutators */
        void setId(std::string id);
        void setComponent(double newComponent, int index, errorCode& status);
        void appendComponent(double newComponent, errorCode& status);
        void concatenateComponents(std::vector<double>& newComponents, errorCode& status);
        void resetComponents(std::vector<double>& newComponents, errorCode& status);

        /* Accessors */
        std::string getId(void);
        double getComponent(int index,errorCode&);
        int getCount(void);
        void print(void);

        /* Usefull functions */
        int compare(Item& x, errorCode& status);
        double innerProduct(Item& x, errorCode& status); 
        double norm(errorCode& status);

        /* Metrices */
        double euclideanDist(Item& x, errorCode& status);
        double cosinDist(Item& x,errorCode& status);
};
// PetropoulakisPanagiotis
