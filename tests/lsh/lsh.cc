#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <string.h>
#include "../../neighborsProblem/utils/utils.h" // For errors etc.
#include "../../neighborsProblem/fileHandler/fileHandler.h" // Read files 
#include "../../neighborsProblem/item/item.h" // Items in sets
#include "../../neighborsProblem/model/lsh/lsh.h" // Models
#include "../../neighborsProblem/model/exhaustiveSearch/exhaustiveSearch.h" // Models

using namespace std;

/* Read arguments from the user */
int readArguments(int argc, char **argv, int& k, int &l, string& inputFile, string& queryFile, string& outputFile);
int scanArguments(int& k, int &l, string& inputFile, string& queryFile, string& outputFile);

int main(int argc, char **argv){
    char delim = ' '; // For data set
    int argumentsProvided; // User provided arguments during compilation 
    double radius;
    list<Item> dataSetPoints, querySetPoints; // Points in data set
    string metrice; // Metrice
    errorCode status; // Errors

    /* Arguments */
    int k = -1, l;
    string inputFile, queryFile, outputFile;

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

    /* Main test */
    model* myModel; // Euclidean or cosin lsh 
    model* optimalModel; // For exhaustive search

    cout << "lsh: Reading data set\n";

    /* Read data set */
    readDataSet(inputFile, 0, delim, dataSetPoints, metrice, status);
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

    /* Create optimal model */
    optimalModel = new exhaustiveSearch();
    
    cout << "lsh: Fitting sub-opt model\n";
    
    /* Fit data set */
    myModel->fit(dataSetPoints,status);
    if(status != SUCCESS){
        delete myModel;
        delete optimalModel;
        printError(status);
        return 0;
    }

    cout << "lsh: Sub-opt model is fitted correctly. Memory consumption is: " << sizeof(myModel) << "\n";
    
    cout << "lsh: Fitting opt model\n";

    /* Fit optimal model */
    optimalModel->fit(dataSetPoints,status);
    if(status != SUCCESS){
        delete myModel;
        delete optimalModel;
        printError(status);
        return 0;
    }

    cout << "lsh: Opt model is fitted correctly. Memory consumption is: " << sizeof(optimalModel) << "\n";

    double nearestDistanceSubOpt, nearestDistanceOpt;
    list<Item>::iterator iter; // Iterate through neighbors 
    Item nearestNeighbor;
    list<Item> radiusNeighbors;
    double mApproximation = -1; // Maximum approximation fraction (dist nearest sub opt / dist nearest opt)

    string inputStr; // Repeat procedure with different query set

    /* Read queries sets, find neighbors and print statistics */
    while(1){

        cout << "lsh: Reading query set\n";

        /* Read query set */
        readQuerySet(queryFile, 0, delim, querySetPoints, radius, status);
        if(status != SUCCESS){
            printError(status);   
            delete myModel;
            delete optimalModel;
            return 0;
        }

        /* Find radius neighbors */
        if(radius != 0){
        
            cout << "lsh: Searching for radius neighbors with given radius: " << radius << "\n";
            
            for(iter = querySetPoints.begin(); iter != querySetPoints.end(); iter++){

                myModel->radiusNeighbors(*iter, radius, radiusNeighbors, NULL, status);
                if(status != SUCCESS){
                    printError(status);
                    delete myModel;
                    delete optimalModel;
                    return 0;
                }
            } // End for - nearest
        }
     
        cout << "lsh: Searching for the nearest neighbor(with opt and sub-opt models)\n";
        
        /* Find nearest neighbor */
        for(iter = querySetPoints.begin(); iter != querySetPoints.end(); iter++){

            myModel->nNeighbor(*iter, nearestNeighbor, &nearestDistanceSubOpt, status);
            if(status != SUCCESS){
                printError(status);
                delete myModel;
                delete optimalModel;
                return 0;
            }
        
            optimalModel->nNeighbor(*iter, nearestNeighbor, &nearestDistanceOpt, status);
            if(status != SUCCESS){
                printError(status);
                delete myModel;
                delete optimalModel;
                return 0;
            }
        
            /* Fix fraction */
            if(mApproximation < nearestDistanceSubOpt / nearestDistanceOpt)
                mApproximation = nearestDistanceSubOpt / nearestDistanceOpt;

        } // End for - nearest

        cout << "lsh: Max approximation fraction: " << mApproximation << "\n";

        cout << "\nDo you want to repeat the procedure with different query set(y/n)?:";
        while(1){
            cin >> inputStr;

            /* Check answer */
            if(inputStr == "y" || inputStr == "n")
                break;
            else
                cout << "Please pres y or n:";
        } // End while

        if(inputStr == "n"){
            cout << "lsh: Terminating\n";
            break;
        }
        else{
            cout << "Give query file name:";
            cin >> inputStr;
            cout << "\n";
        }

    } // End while

    /* Delete models */
    delete optimalModel;    
    delete myModel;

    return 0;
}

/* Read arguments from the user */
/* No arguments: 0              */
/* Arguments provided: 1        */
/* Invalid arguments: -1        */
int readArguments(int argc, char **argv, int& k, int &l, string& inputFile, string& queryFile, string& outputFile){

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
int scanArguments(int& k, int &l, string& inputFile, string& queryFile, string& outputFile){
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
