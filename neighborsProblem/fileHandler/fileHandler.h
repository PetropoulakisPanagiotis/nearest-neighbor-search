#pragma once
#include <vector>
#include <list>
#include "../utils/utils.h"
#include "../item/item.h"

/* Functions for reading data set, query set and for */
/* writing results in files                          */

/* Read given file, extract items and read possible metrices(euclidean, cosin, etc) */
void readDataSet(std::string fileName, int withId, char delim, std::list<Item>& points, std::string& types, errorCode& status); 

/* Read given file, extract items and read possible radius */
void readQuerySet(std::string fileName, int withId, char delim, std::list<Item>& points, double& radius, errorCode& status); 
// Petropoulakis Panagiotis
