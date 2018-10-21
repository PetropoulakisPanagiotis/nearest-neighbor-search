#pragma once
#include <vector>
#include <list>
#include "../utils/utils.h"
#include "../item/item.h"

/* Functions for reading data set, query set and for */
/* writing results in files                          */

/* Read given file, extract points and read possible metrices(euclidean, cosin, etc) */
void readDataSet(std::string fileName, std::vector<Item>& points, std::list<std::string> types, errorCode& status, int withId); 

// Petropoulakis Panagiotis
