#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <chrono>
#include <string.h>
#include "../../neighborsProblem/utils/utils.h" // For errors etc.
#include "../../neighborsProblem/fileHandler/fileHandler.h" // Read files 
#include "../../neighborsProblem/item/item.h" // Items in sets
#include "../../neighborsProblem/model/lsh/lsh.h" // Models
#include "../../neighborsProblem/model/exhaustiveSearch/exhaustiveSearch.h" // Models

using namespace std;

/* Read arguments from the user */
int readArguments(int argc, char **argv, int& k, int& l, string& inputFile, string& queryFile, string& outputFile);
int scanArguments(int& k, int& l, string& inputFile, string& queryFile, string& outputFile);

int main(int argc, char **argv){
    char delim = ' '; // For data set
    int argumentsProvided; // User provided arguments 
    double radius;
    list<Item> dataSetPoints, querySetPoints; // Points in data set
    string metrice; // Metrice
    errorCode status; // Errors

    /* Arguments */
    int k = -1, l;
    string inputFile, queryFile, outputFile;
    ofstream resultsFile; 
    
    /* Read possible arugments from the user */
    argumentsProvided = readArguments(argc, argv, k, l, inputFile, queryFile, outputFile);
    if(argumentsProvided == -1){
        cout << "Please give valid arguments. Try again later\n";
        return 0;
    }
   
    cout << "Welcome to lsh search\n";
    cout << "-----------------------\n\n";
    
    /* Scan arguments from the stdin */
    if(argumentsProvided == 0){
        argumentsProvided = scanArguments(k, l, inputFile, queryFile, outputFile);
        if(argumentsProvided == -1){
            cout << "Please give valid arguments. Try again later\n";
            return 0;
        }
    }

    /* Models to be tested */
    model* myModel; // Euclidean or cosin lsh 
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
    double avgTimeNearest = 0; 
    int flag = 0;

    string inputStr;
    int fitAgain = 0, newQuery = 0;

    /* Read queries sets, find neighbors and print statistics */
    while(1){

        if(fitAgain == 0{
            
            cout << "lsh: Reading data set\n";

            /* Read data set */
            readDataSet(inputFile, 1, delim, dataSetPoints, metrice, status);
            if(status != SUCCESS){
                printError(status);
                return 0;
            }

            /* Create model */
            if(metrice == "euclidean"){
                if(k != -1)
                    myModel = new lshEuclidean(k,l, status);
                else
                    myModel = new lshEuclidean();
            }
            else if(metrice == "cosin"){
                if(k != -1)
                    myModel = new lshCosin(k,l, status);
                else
                    myModel = new lshCosin();
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
            optimalModel = new exhaustiveSearch();
            if(optimalModel == NULL){
                status = ALLOCATION_FAILED;
                printError(status);
                delete myModel;
                return -1;
            }

            cout << "lsh: Fitting sub-opt model\n";
            
            /* Fit data set */
            myModel->fit(dataSetPoints,status);
            if(status != SUCCESS){
                delete myModel;
                delete optimalModel;
                printError(status);
                return 0;
            }

            cout << "lsh: Sub-opt model is fitted correctly. Memory consumption is: " << myModel->size() << " bytes\n";
            
            cout << "lsh: Fitting opt model\n";

            /* Fit optimal model */
            optimalModel->fit(dataSetPoints,status);
            if(status != SUCCESS){
                delete myModel;
                delete optimalModel;
                printError(status);
                return 0;
            }

            cout << "lsh: Opt model is fitted correctly. Memory consumption is: " << optimalModel->size() << " bytes\n";
        } 

        if(newQuery = 0{

            cout << "lsh: Reading query set\n";

            /* Read query set */
            readQuerySet(queryFile, 0, delim, querySetPoints, radius, status);
            if(status != SUCCESS){
                printError(status);   
                delete myModel;
                delete optimalModel;
                return 0;
            }
        }

        cout << "lsh: Opening output file\n";

        /* Truncate if file exists */
        resultsFile.open(outputFile, ios::trunc);
        if(!resultsFile){
            cout << "Can't open given output file\n";
            delete myModel;
            delete optimalModel;
            return 0;
        }

            
        cout << "lsh: Searching for neighbors with given radius: " << radius << "\n";

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
            
            /* Radius exists */
            if(radius != 0){
                resultsFile << "R-near neighbors:\n";

                /* R-neighbors */
                for(iterNeighbors = radiusNeighbors.begin(); iterNeighbors != radiusNeighbors.end(); iterNeighbors++)
                   resultsFile << iterNeighbors->getId() << "\n";     
            }
            /* Nearest */ 
            resultsFile << "Nearest neighbor: " << nearestNeighborSubOpt.getId() << "\n";
            resultsFile << "distanceLSH: " << nearestDistanceSubOpt << "\n";
            resultsFile << "distanceTrue: " << nearestDistanceOpt << "\n"; 
            resultsFile << "tLSH: " << chrono::duration_cast<chrono::microseconds>(endSubOpt - beginSubOpt).count() / 1000000.0 << " sec\n";
            resultsFile << "tTrue: " << chrono::duration_cast<chrono::microseconds>(endOpt - beginOpt).count() / 1000000.0 << " sec\n"; 

            if(flag == 0 && nearestDistanceSubOpt != -1)
                avgTimeNearest = chrono::duration_cast<chrono::microseconds>(endSubOpt - beginSubOpt).count() / 1000000.0;
            else if(nearestDistanceSubOpt != -1){
                avgTimeNearest += chrono::duration_cast<chrono::microseconds>(endSubOpt - beginSubOpt).count() / 1000000.0;
                avgTimeNearest /= 2;
            }

            resultsFile << "\n";

            flag = 1;
        } // End for - query points  

        if(mApproximation == -1)
            cout << "lsh: Can't find nearest neighbors for given data set\n";
        else
            cout << "lsh: Max approximation fraction: " << mApproximation << "\n";

        cout << "lsh: Average time for nearest neighbors: " << avgTimeNearest << " sec\n";
            
        cout << "lsh: Closing output file: " << outputFile << "\n";
        
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
            cout << "lsh: Deleting models\n";

            /* Delete models */
            delete optimalModel;    
            delete myModel;
            
            cout << "lsh: Terminating\n";
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
                cout << "lsh: Deleting models\n";

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
int readArguments(int argc, char **argv, int& k, int& l, string& inputFile, string& queryFile, string& outputFile){

    /* No argumets */
    if(argc == 1)
        return 0;

    /* Invalid arguments */
    if(argc != 11)
        return -1;

    if(strcmp(argv[1], "-d") || strcmp(argv[3], "-q") || strcmp(argv[5], "-k") || strcmp(argv[7], "-L") || strcmp(argv[9], "-o"))
        return -1;

    /* Copy arguments */
    try{
        k = stoi(argv[6]);
    }
    catch(...){
        return -1;
    }

    try{
        l = stoi(argv[8]);
    }
    catch(...){
        return -1;
    }

    inputFile = argv[2];
    queryFile = argv[4];
    outputFile = argv[10];

    return 1;
}

/* Read arguments from stdin */
/* Arguments provided: 1     */
/* Invalid arguments: -1     */
int scanArguments(int& k, int& l, string& inputFile, string& queryFile, string& outputFile){
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

    cout << "Do you want to provide hyperparameters for lsh(y/n)?:";
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

    cout << "Give l hyperparameter:";
    
    cin >> inputStr;

    try{
        l = stoi(inputStr);
    }
    catch(...){
        return -1;
    }

    cout << "\n";
    
    return 1;
}

// Petropoulakis Panagiotis
