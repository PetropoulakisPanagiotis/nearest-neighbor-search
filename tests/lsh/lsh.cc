#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <string.h>
#include "../../neighborsProblem/utils/utils.h"
#include "../../neighborsProblem/fileHandler/fileHandler.h"
#include "../../neighborsProblem/item/item.h"
#include "../../neighborsProblem/model/model.h"

using namespace std;

/* Read arguments from the user */
int readArguments(int argc, char **argv, int& k, int &l, string& inputFile, string& queryFile, string& outputFile);
int scanArguments(int& k, int &l, string& inputFile, string& queryFile, string& outputFile);

int main(int argc, char **argv){
    list<Item> dataSetPoints, querySetPoints; // Points in data set
    string metrice; // Metrices
    list<string>::iterator iterMetrices;
    char delim = ' ';
    int argumentsProvided; 
    errorCode status;
    double radius;

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
    
    /* Scan arguments from stdin */
    if(argumentsProvided == 0){
        argumentsProvided = scanArguments(k, l, inputFile, queryFile, outputFile);
        if(argumentsProvided == -1){
            cout << "Please give valid arguments. Try again later\n";
            return 0;
        }
    }

    /* Read data set */
    readDataSet(inputFile, 0, delim, dataSetPoints, metrice, status);
    if(status != SUCCESS){
        printError(status);
        return 0;
    }

    model* myModel;

    /* Create model */
    if(metrice == "euclidean"){
        if(k != -1)
            myModel = new lshEuclidean(k,l);
        else
            myModel = new lshEuclidean();
    }
    else if(metrice == "cosin"){
        if(k != -1)
            myModel = new lshCosin(k,l);
        else
            myModel = new lshCosin();
    }
    
    /* Fit data set */
    myModel->fit(dataSetPoints,status);
    if(status != SUCCESS){
        delete myModel;
        printError(status);
        return 0;
    }

    
    /* Read query set */
    readQuerySet(queryFile, 0, delim, querySetPoints, radius, status);
    if(status != SUCCESS){
        printError(status);
        return 0;
    }

    /*

    Item* tmp; vector<double> comp;
    list<Item> radiusNeighbors;
    list<double> neighborsDistances;

    int i;

    for(i = 0; i < 128; i++){
     
        if(i < 50)
            comp.push_back(i);
        else
            comp.push_back(i - 40);
    }

    tmp = new Item(comp,status);

    myModel->radiusNeighbors(*tmp,10000,radiusNeighbors, NULL, status);


    list<Item>::iterator iter;
    for(iter= radiusNeighbors.begin(); iter != radiusNeighbors.end(); iter++)
        iter->print();
        
    delete tmp;
   */ 

    /* Find nearest neighbor */

    /* Delete model */
    //delete myModel;


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

    cout << "Give input file name:\n";
    cin >> inputStr;

    inputFile = inputStr;

    cout << "Give query file name:\n";
    cin >> inputStr;

    queryFile = inputStr;

    cout << "Give output file name:\n";
    cin >> inputStr;

    outputFile = inputStr;

    cout << "Do you want to provide hyperparameters for lsh(y/n):\n";
    while(1){
        cin >> inputStr;

        /* Check answer */
        if(inputStr == "y" || inputStr == "n")
            break;
        else
            cout << "Please pres y or n:\n";
    } // End while

    if(inputStr == "n")
        return 1;

    cout << "Give k hyperparameter\n";
    
    cin >> inputStr;

    try{
        k = stoi(inputStr);
    }
    catch(...){
        return -1;
    }

    cout << "Give l hyperparameter\n";
    
    cin >> inputStr;

    try{
        l = stoi(inputStr);
    }
    catch(...){
        return -1;
    }

    return 1;
}

// Petropoulakis Panagiotis
