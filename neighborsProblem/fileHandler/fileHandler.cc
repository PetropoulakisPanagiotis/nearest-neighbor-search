#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include "../item/item.h"
#include "../utils/utils.h"
#include "fileHandler.h"

using namespace std;

/* Read given file, extract points and read possible metrices(euclidean, cosine, etc) */
/* WithId == 0, points have id's                                                     */
/* WithId == 1, points havn't id's                                                   */
void readDataSet(string fileName, int withId, char delim, list<Item>& points, string& types, errorCode& status){
    ifstream file; 
    string line, word; // Line is splitted in words
    int flag = 0; // Check only once for metrices
    int i, j, wordsSize, specialChar;
    double currComponent;
    
    /* Structures */
    vector<string> metrices {"euclidean" , "cosine"}; // Available metrices
    set<string> ids; // Keep all ids - Check if all ids are unique
    set<string>::iterator iterSet; // Iterate through ids
    string::iterator iterStr; // Iteratre through word

    /* Note: words in file == points/id/metrices */
    status = SUCCESS;

    /* Check parameters */
    if(fileName.length() == 0 || (withId != 1 && withId != 0)){
        status = INVALID_PARAMETERS;
        return;
    }

    /* Clear points, types */
    points.clear();
    types.clear();

    file.open(fileName);
    
    /* Check if file opened properly */
    if(!file){
        status = INVALID_DATA_SET;
        return;
    }

    /* Read lines in file */
    while(getline(file, line)){
        /* Discard empty lines */
        if(line.length() == 0)
            continue;

        /* Check for metrices */
        if(flag == 0){
            for(i = 0; i < (int)metrices.size(); i++){
                for(j = 0; j < (int)metrices[i].length(); j++){
                    if(line[j] != metrices[i][j])
                        break;
                }

                if(j == (int)metrices[i].length()){
                    types = metrices[i];
                    flag = 1;
                    break;
                }
            } // End for
            
            if(flag == 1)
                continue;

            /* No metrices found - Reset words */
            types = "euclidean"; // Default metrice
            flag = 1;
        }  // End if metrices

        /* For current points in line */
        vector<double> components;
        string id; 

        /* Split line */
        std::istringstream wordStream(line);
        vector<string> words; 

        /* Get points */
        while(getline(wordStream, word, delim))
            words.push_back(word);
       
        /* Receive id's - First word in line */
        if(withId == 1){
            iterSet = ids.find(words[0]); 

            /* Id exists - Invalid file */
            if(iterSet != ids.end()){
                status = INVALID_DATA_SET;
                return;
            }
            
            /* Add id in set */
            ids.insert(words[0]);

            /* Keep currend id */
            id = words[0];
        } // End if - id       
    
        string currWord; // Change form of word
        wordsSize = words.size();
        
        /* Read components */
        for(i = withId; i < wordsSize; i++){
            
            /* Handle word */
            currWord = words[i];
            
            specialChar = 0;

            /* Discard special characters */
            iterStr = currWord.begin();
            while(iterStr != currWord.end()){
                if(*iterStr < '0' || *iterStr > 'z'){
                    specialChar = 1;
                    break;
                }
                iterStr++;
            }

            /* Special characters found */
            if(specialChar == 1)
                continue;
            
            /* Handle negative numbers */
            if(currWord[0] == '-')
                currWord.erase(0,1);
          
            /* Handle floating points */
            currWord.erase(remove(currWord.begin(), currWord.end(), '.'), currWord.end());

            /* Check if component is digit */
            iterStr = currWord.begin();
            while(iterStr != currWord.end() && isdigit(*iterStr))
                iterStr++;
           
            /* Not digit */
            if(currWord.size() == 0 || iterStr != currWord.end()){
                status = INVALID_DATA_SET;
                return;
            }

            /* Convert string to double */
            try{
                currComponent = stod(words[i]);
            }
            catch(...){
                status = INVALID_POINTS;
                return;
            }

            /* Valid component - Save it */
            components.push_back(currComponent);
        } // End for
        
        /* Number of points is too big */
        if(points.size() == MAX_POINTS){
            status = INVALID_POINTS;
            return;
        }

        /* Create new item */
        if(withId == 1){      
            points.push_back(Item(id,components,status));
            if(status != SUCCESS){
                return;
            }
        }
        else{
            points.push_back(Item(components,status));
            if(status != SUCCESS){
                return;
            }
        }
        
    } // End while - Read line

    /* Small number of points */
    if(points.size() < MIN_POINTS){
        status = INVALID_POINTS;
        return;
    }

    file.close();
}

/* Read given file, extract points and read possible radius */
/* WithId == 0, points have id's                            */
/* WithId == 1, points havn't id's                          */
void readQuerySet(string fileName, int withId, char delim, list<Item>& points, double& radius, errorCode& status){
    ifstream file; 
    string line, word; // Line is splitted in words
    int flag = 0; // Check only once for metrices
    int i, wordsSize, specialChar;
    double currComponent;
    
    /* Structures */
    set<string> ids; // Keep all ids - Check if all ids are unique
    set<string>::iterator iterSet; // Iterate through ids
    string::iterator iterStr; // Iteratre through word

    /* Note: words in file == points/id/metrices */
    status = SUCCESS;

    /* Check parameters */
    if(fileName.length() == 0 || (withId != 1 && withId != 0)){
        status = INVALID_PARAMETERS;
        return;
    }

    /* Clear points */
    points.clear();

    file.open(fileName);
    
    /* Check if file opened properly */
    if(!file){
        status = INVALID_DATA_SET;
        return;
    }

    /* Read lines in file */
    while(getline(file, line)){
        /* Discard empty lines */
        if(line.length() == 0)
            continue;

        /* Check for radius */
        if(flag == 0){
             
            /* Split line */
            std::istringstream wordStream(line);
            vector<string> words; 
            
            /* Get points */
            while(getline(wordStream, word, ' '))
                words.push_back(word);
            
            if(words[0] == "Radius:"){
                if(words.size() > 1){
                    try{
                        radius = stod(words[1]);
                    }
                    catch(...){
                        status = INVALID_DATA_SET;
                        return;
                    }
                    flag = 1;
                    continue;
                }
            }

            /* No radius */
            radius = 0; // Default radius - Find only nearest neighbor
            flag = 1;
        }  // End if metrices

        /* For current points in line */
        vector<double> components;
        string id; 

        /* Split line */
        std::istringstream wordStream(line);
        vector<string> words; 

        /* Get points */
        while(getline(wordStream, word, delim))
            words.push_back(word);
       
        /* Receive id's - First word in line */
        if(withId == 1){
            iterSet = ids.find(words[0]); 

            /* Id exists - Invalid file */
            if(iterSet != ids.end()){
                status = INVALID_DATA_SET;
                return;
            }
            
            /* Add id in set */
            ids.insert(words[0]);

            /* Keep currend id */
            id = words[0];
        } // End if - id       
    
        string currWord; // Change form of word
        wordsSize = words.size();
        
        /* Read components */
        for(i = withId; i < wordsSize; i++){
            
            /* Handle word */
            currWord = words[i];
            
            specialChar = 0;

            /* Discard special characters */
            iterStr = currWord.begin();
            while(iterStr != currWord.end()){
                if(*iterStr < '0' || *iterStr > 'z'){
                    specialChar = 1;
                    break;
                }
                iterStr++;
            }

            /* Special characters found */
            if(specialChar == 1)
                continue;
            
            /* Handle negative numbers */
            if(currWord[0] == '-')
                currWord.erase(0,1);
          
            /* Handle floating points */
            currWord.erase(remove(currWord.begin(), currWord.end(), '.'), currWord.end());

            /* Check if component is digit */
            iterStr = currWord.begin();
            while(iterStr != currWord.end() && isdigit(*iterStr))
                iterStr++;
           
            /* Not digit */
            if(currWord.size() == 0 || iterStr != currWord.end()){
                status = INVALID_DATA_SET;
                return;
            }

            /* Convert string to double */
            try{
                currComponent = stod(words[i]);
            }
            catch(...){
                status = INVALID_POINTS;
                return;
            }

            /* Valid component - Save it */
            components.push_back(currComponent);
        } // End for
        
        /* Create new item */
        if(withId == 1){      
            points.push_back(Item(id,components,status));
            if(status != SUCCESS){
                return;
            }
        }
        else{
            points.push_back(Item(components,status));
            if(status != SUCCESS){
                return;
            }
        }
    } // End while - Read line

    file.close();
}

// Petropoulakis Panagiotis
