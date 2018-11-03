#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <chrono>
#include <new>
#include <string.h>
#include "../../neighborsProblem/utils/utils.h" // For errors etc.
#include "../../neighborsProblem/fileHandler/fileHandler.h" // Read files 
#include "../../neighborsProblem/item/item.h" // Items in sets
#include "../../neighborsProblem/model/hypercube/hypercube.h" // Models
#include "../../neighborsProblem/model/exhaustiveSearch/exhaustiveSearch.h" // Models

using namespace std;

/* Read arguments from the user */
int readArguments(int argc, char **argv, int& k, int& m, int& probes, string& inputFile, string& queryFile, string& outputFile);
int scanArguments(int& k, int& m, int& probes, string& inputFile, string& queryFile, string& outputFile);

int main(int argc, char **argv){
    char delim = ' '; // For data set
    int argumentsProvided; // User provided arguments
    double radius;
    list<Item> dataSetPoints, querySetPoints; // Points in data set
    string metrice; // Metrice
    errorCode status; // Errors

    /* Arguments */
    int k = -1, m, probes;
    string inputFile, queryFile, outputFile;
    ofstream resultsFile; 

    /* Read possible arugments from the user */
    argumentsProvided = readArguments(argc, argv, k, m, probes, inputFile, queryFile, outputFile);
    if(argumentsProvided == -1){
        cout << "Please give valid arguments. Try again later\n";
        return 0;
    }

    if(argumentsProvided == 1 && k < 0){
        cout << "Please give valid arguments. Try again later\n";
        return 0;
    }

    cout << "Welcome to cube search\n";
    cout << "-----------------------\n\n";

    /* Scan arguments from the stdin */
    if(argumentsProvided == 0){
        argumentsProvided = scanArguments(k, k, m, inputFile, queryFile, outputFile);
        if(argumentsProvided == -1){
            cout << "Please give valid arguments. Try again later\n";
            return 0;
        }
    }

    /* Models to be tested */
    model* myModel; // Euclidean or cosine cube 
    model* optimalModel; // For exhaustive search

    double nearestDistanceSubOpt, nearestDistanceOpt;
    list<Item>::iterator iterQueries; // Iterate through queries 
    list<Item>::iterator iterNeighbors; // Iterate through neighbors 
    Item nearestNeighborSubOpt, nearestNeighborOpt;
    list<Item> radiusNeighbors;

    double mApproximation = -1; // Maximum approximation fraction (dist nearest sub opt / dist nearest opt)

    /* Measure time */
    chrono::steady_clock::time_point beginOpt, endOpt;
    chrono::steady_clock::time_point beginSubOpt, endSubOpt;
    double avgTimeNearestSubOpt = 0; 
    double avgTimeNearestOpt = 0; 

    int flag = 0;
    int flag1 = 0;

    string inputStr; // Read new files from the user  
    int fitAgain = 0, newQuery = 0;

    /* Read queries sets, find neighbors and print statistics */
    while(1){


        if(fitAgain == 0){
            cout << "cube: Reading data set\n";

            /* Read data set */
            readDataSet(inputFile, 1, delim, dataSetPoints, metrice, status);
            if(status != SUCCESS){
                printError(status);
                return 0;
            }

            /* Create model */
            if(metrice == "euclidean"){
                if(k != -1)
                    myModel = new hypercubeEuclidean(k, m, probes, status);
                else
                    myModel = new hypercubeEuclidean();
            }
            else if(metrice == "cosine"){
                if(k != -1)
                    myModel = new hypercubeCosine(k, m, probes, status);
                else
                    myModel = new hypercubeCosine();
            }

            if(status != SUCCESS){
                printError(status);
                delete myModel;
                return -1;
            }

            if(myModel == NULL){
                status = ALLOCATION_FAILED;
                printError(status);
                return -1;
            }
            
            /* Create optimal model */
            optimalModel = new exhaustiveSearch(metrice);
            if(optimalModel == NULL){
                status = ALLOCATION_FAILED;
                printError(status);
                delete myModel;
                return -1;
            }

            cout << "cube: Fitting sub-opt model\n";
            
            /* Fit data set */
            myModel->fit(dataSetPoints,status);
            if(status != SUCCESS){
                delete myModel;
                delete optimalModel;
                printError(status);
                return 0;
            }

            cout << "cube: Sub-opt model is fitted correctly. Memory consumption is: " << myModel->size() << " bytes\n";
            
            cout << "cube: Fitting opt model\n";

            /* Fit optimal model */
            optimalModel->fit(dataSetPoints,status);
            if(status != SUCCESS){
                delete myModel;
                delete optimalModel;
                printError(status);
                return 0;
            }

            cout << "cube: Opt model is fitted correctly. Memory consumption is: " << optimalModel->size() << " bytes\n";
        }

        if(newQuery == 0){
            cout << "cube: Reading query set\n";

            /* Read query set */
            readQuerySet(queryFile, 0, delim, querySetPoints, radius, status);
            if(status != SUCCESS){
                printError(status);   
                delete myModel;
                delete optimalModel;
                return 0;
            }
        }

        cout << "cube: Opening output file\n";

        /* Truncate if file exists */
        resultsFile.open(outputFile, ios::trunc);
        if(!resultsFile){
            cout << "Can't open given output file\n";
            delete myModel;
            delete optimalModel;
            return 0;
        }

            
        cout << "cube: Searching for neighbors with given radius: " << radius << "\n";

        /* Find neighbors */
        for(iterQueries = querySetPoints.begin(); iterQueries != querySetPoints.end(); iterQueries++){

            /* Find radius */
            if(radius != 0){
                myModel->radiusNeighbors(*iterQueries, radius, radiusNeighbors, NULL, status);
                if(status != SUCCESS){
                    printError(status);
                    delete myModel;
                    delete optimalModel;
                    return 0;
                }
           
            }

            /* Find nearest */
            beginSubOpt = chrono::steady_clock::now();
            myModel->nNeighbor(*iterQueries, nearestNeighborSubOpt, &nearestDistanceSubOpt, status);
            if(status != SUCCESS){
                printError(status);
                delete myModel;
                delete optimalModel;
                return 0;
            }
            endSubOpt = chrono::steady_clock::now();
           
            beginOpt = chrono::steady_clock::now();
            optimalModel->nNeighbor(*iterQueries, nearestNeighborOpt, &nearestDistanceOpt, status);
            if(status != SUCCESS){
                printError(status);
                delete myModel;
                delete optimalModel;
                return 0;
            }
            endOpt = chrono::steady_clock::now();
            
            /* Fix fraction */
            if(nearestDistanceSubOpt != -1 && mApproximation < nearestDistanceSubOpt / nearestDistanceOpt)
                mApproximation = nearestDistanceSubOpt / nearestDistanceOpt;

            ///////////////////////////
            /* Write results in file */
            ///////////////////////////

            /* Print id */
            resultsFile << "Query: " << iterQueries->getId() << "\n";

            /* Print radius */
            if(radius != 0){
                resultsFile << "R-near neighbors:\n";

                /* R-neighbors */
                for(iterNeighbors = radiusNeighbors.begin(); iterNeighbors != radiusNeighbors.end(); iterNeighbors++)
                    resultsFile << iterNeighbors->getId() << "\n";     
            }

            /* Nearest */ 
            resultsFile << "Nearest neighbor: " << nearestNeighborSubOpt.getId() << "\n";
            resultsFile << "distanceCube: " << nearestDistanceSubOpt << "\n";
            resultsFile << "distanceTrue: " << nearestDistanceOpt << "\n"; 
            resultsFile << "tCube: " << chrono::duration_cast<chrono::microseconds>(endSubOpt - beginSubOpt).count() / 1000000.0 << "sec\n";
            resultsFile << "tTrue: " << chrono::duration_cast<chrono::microseconds>(endOpt - beginOpt).count() / 1000000.0 << "sec\n"; 

            if(flag == 0 && nearestDistanceSubOpt != -1)
                avgTimeNearestSubOpt = chrono::duration_cast<chrono::microseconds>(endSubOpt - beginSubOpt).count() / 1000000.0;
            else if(nearestDistanceSubOpt != -1){
                avgTimeNearestSubOpt += chrono::duration_cast<chrono::microseconds>(endSubOpt - beginSubOpt).count() / 1000000.0;
                avgTimeNearestSubOpt /= 2;
                flag = 1;
            }           
           
            if(flag1 == 0)
                avgTimeNearestOpt = chrono::duration_cast<chrono::microseconds>(endOpt - beginOpt).count() / 1000000.0;
            else if(nearestDistanceOpt != -1){
                avgTimeNearestOpt += chrono::duration_cast<chrono::microseconds>(endOpt - beginOpt).count() / 1000000.0;
                avgTimeNearestOpt /= 2;
                flag1 = 1;
            }

            
            resultsFile << "\n";
        } // End for - query points 
     
        if(mApproximation == -1)
            cout << "cube: Can't find nearest neighbors for given data set\n";
        else
            cout << "cube: Max approximation fraction: " << mApproximation << "\n";

        cout << "cube: Average time for nearest neighbors - sub opt: " << avgTimeNearestSubOpt << " sec\n";
        cout << "cube: Average time for nearest neighbors opt: " << avgTimeNearestOpt << " sec\n";
       
        cout << "cube: Deleting models\n";

        cout << "cube: Closing output file: " << outputFile << "\n";
        
        cout << "\nDo you want to repeat the procedure with different sets(y/n)?:";
        while(1){
            cin >> inputStr;

            /* Check answer */
            if(inputStr == "y" || inputStr == "n")
                break;
            else
                cout << "Please pres y or n:";
        } // End while

        if(inputStr == "n"){
            cout << "cube: Deleting models\n";

            /* Delete models */
            delete optimalModel;    
            delete myModel;
            
            cout << "cube: Terminating\n";
            break;
        }
        else{       

            resultsFile.close();
            
            cout << "\nDo you want to fit different data set(y/n)?:";
            while(1){
                cin >> inputStr;

                /* Check answer */
                if(inputStr == "y" || inputStr == "n")
                    break;
                else
                cout << "Please pres y or n:";
            } // End while

            if(inputStr == "n")
                fitAgain = 1;
            else{
                fitAgain = 0;
                cout << "cube: Deleting models\n";

                /* Delete models */
                delete optimalModel;    
                delete myModel;
                
                cout << "Give input file name:";
                cin >> inputStr;
                inputFile = inputStr;
            }

            cout << "\nDo you want to give different query set(y/n)?:";
            while(1){
                cin >> inputStr;

                /* Check answer */
                if(inputStr == "y" || inputStr == "n")
                    break;
                else
                cout << "Please pres y or n:";
            } // End while

            if(inputStr == "n")
                newQuery = 1;
            else{
                
                newQuery = 0;
                cout << "Give query file name:";
                cin >> inputStr;

                queryFile = inputStr;
            }

            cout << "Give output file name:";
            cin >> inputStr;

            cout << "\n";
        }
    } // End while

    return 0;
}

/* Read arguments from the user */
/* No arguments: 0              */
/* Arguments provided: 1        */
/* Invalid arguments: -1        */
int readArguments(int argc, char **argv, int& k, int& m, int& probes, string& inputFile, string& queryFile, string& outputFile){

    /* No argumets */
    if(argc == 1)
        return 0;

    /* Invalid arguments */
    if(argc != 13)
        return -1;

    if(strcmp(argv[1], "-d") || strcmp(argv[3], "-q") || strcmp(argv[5], "-k") || strcmp(argv[7], "-M") || strcmp(argv[9], "-probes") || strcmp(argv[11], "-o"))
        return -1;

    /* Copy arguments */
    try{
        k = stoi(argv[6]);
    }
    catch(...){
        return -1;
    }

    try{
        m = stoi(argv[8]);
    }
    catch(...){
        return -1;
    
    }
    try{
        probes = stoi(argv[10]);
    }
    catch(...){
        return -1;
    
    }

    inputFile = argv[2];
    queryFile = argv[4];
    outputFile = argv[12];

    return 1;
}

/* Read arguments from stdin */
/* Arguments provided: 1     */
/* Invalid arguments: -1     */
int scanArguments(int& k, int &m, int& probes, string& inputFile, string& queryFile, string& outputFile){
    string inputStr;

    cout << "Give input file name:";
    cin >> inputStr;

    inputFile = inputStr;

    cout << "Give query file name:";
    cin >> inputStr;

    queryFile = inputStr;

    cout << "Give output file name:";
    cin >> inputStr;

    outputFile = inputStr;

    cout << "Do you want to provide hyperparameters for cube(y/n)?:";
    while(1){
        cin >> inputStr;

        /* Check answer */
        if(inputStr == "y" || inputStr == "n")
            break;
        else
            cout << "Please pres y or n:";
    } // End while

    if(inputStr == "n"){
        cout << "\n";
        return 1;
    }

    cout << "Give k hyperparameter:";
    
    cin >> inputStr;

    try{
        k = stoi(inputStr);
    }
    catch(...){
        return -1;
    }

    cout << "Give m hyperparameter:";
    
    cin >> inputStr;

    try{
        m = stoi(inputStr);
    }
    catch(...){
        return -1;
    }

    cout << "Give probes hyperparameter:";
    
    cin >> inputStr;

    try{
        probes = stoi(inputStr);
    }
    catch(...){
        return -1;
    }

    cout << "\n";
    
    return 1;
}

// Petropoulakis Panagiotis
