#include <iostream>
#include <string>
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

/* Read given file, extract points and read possible metrices(euclidean, cosin, etc) */
/* WithId == 0, points have id's                                                     */
/* WithId == 1, points havn't id's                                                   */
void readDataSet(string fileName, int withId, char delim, vector<Item>& points, list<string> types, errorCode& status){
    ifstream file; 
    string line, word; // Line is splitted in words
    int flag = 0; // Check only once for metrices
    int i, wordsSize, specialChar;
    double currComponent;
    
    /* Structures */
    vector<string> metrices {"euclidean" , "cosin"}; // Available metrices
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

        /* For current points in line */
        vector<double> components;
        string id; 

        /* Check for metrices */
        if(flag == 0){
            
            /* Split line */
            std::istringstream wordStream(line);
            vector<string> words; 
            
            while(getline(wordStream, word, ',')){
                /* Remove spaces */
                word.erase(remove(word.begin(), word.end(), ' '), word.end());

                /* Add word */
                words.push_back(word);
            } // End while - split

            /* Check if metrices are valid */
            if(words.size() <= 2){
                if(words[0] == metrices[0] || words[0] == metrices[1]){
                    
                    /* Two metrices */
                    if(words.size() == 2 && words[0] != words[1] && (words[1] == metrices[0] || words[1] == metrices[1])){
                        types.push_back(words[0]);
                        types.push_back(words[1]);
                        continue;
                    }
                    /* One metrice */
                    else if(words.size() == 2 && words[0] == words[1]){
                        types.push_back(words[0]);
                        continue;
                    }
                }
            } // End if available metrices

            /* No metrices found - Reset words */
            words.clear();
            flag = 1;
        }  // End if metrices

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

        /* Create new item and add it */
        if(withId == 1){
            points.push_back(Item(id,components,status));
            if(status != SUCCESS)
                return; 
        }
        else{
            points.push_back(Item(components,status));
            if(status != SUCCESS)
                return; 
        }
    } // End while - Read line

    /* Small number of points */
    if(points.size() < MIN_POINTS){
        status = INVALID_POINTS;
        return;
    }

    file.close();
}

// Petropoulakis Panagiotis
