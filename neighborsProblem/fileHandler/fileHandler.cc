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
void readDataSet(string fileName, vector<Item>& points, list<string> types, errorCode& status, int withId){
    ifstream file;
    string line;
    string word;
    vector<string> metrices {"euclidean" , "cosin"};
    set<string> ids; // Keep all ids - Check if all ids are unique
    set<string>::iterator iterSet;
    string::iterator iterStr;
    int flag = 0;
    int i;

    /* Clear points, types */
    points.clear();
    types.clear();

    file.open(fileName);

    /* Read lines in file */
    while(getline(file, line)){
        
        /* Discard empty lines */
        if(line.length() == 0)
            continue;

        /* For new items */
        vector<double> components;
        string id;

        /* Split line */
        std::istringstream wordStream(line);
        vector<string> words;
    
        /* Check for metrices */
        if(flag == 0){
            
            while(getline(wordStream, word, ',')){
                /* Remove spaces */
                word.erase(remove(word.begin(), word.end(), ' '), word.end());

                /* Add word */
                words.push_back(word);
            }

            if(words.size() <= 2){
                if(words[0] == metrices[0] || words[0] == metrices[1]){
                   
                    if(words.size() == 2 && words[0] != words[1] && (words[1] == metrices[0] || words[1] == metrices[1])){
                        types.push_back(words[0]);
                        types.push_back(words[1]);
                        continue;
                    }
                    else if(words.size() == 2 && words[0] == words[1]){
                        types.push_back(words[0]);
                        continue;
                    }
                }
            }
            words.clear();
            flag = 1;
        }   

        /* Get words */
        while(getline(wordStream, word, ' '))
            words.push_back(word);
        
        /* Check id's */
        if(withId == 0){
            iterSet = ids.find(words[0]);

            /* Id exists */
            if(iterSet != ids.end()){
                status = INVALID_DATA_SET;
                return;
            }
            
            /* Add id */
            ids.insert(words[0]);
            id = words[0];

            /* Read components */
            for(i = 1; i < words.size(); i++){
                iterStr = words[i].begin();
                while(iterStr != words[i].end() && isdigit(*iterStr))
                    iterStr++;

                /* Not digit */
                if(iterStr != words[i].end()){
                    status = INVALID_DATA_SET;
                    return;
                }

                /* Add component */
                components.push_back(stoi(words[i]));
            } // End for
        }
        else{
            /* Read components */
            for(i = 0; i < words.size(); i++){
                
                //istringstream iss(words[i]);
                double num;
                
                /* Not string */
                //if((iss >> num).fail()){
                  //  status = INVALID_DATA_SET;
                   // return;
                //} 

                cout << words[i];    
                /* Add component */
                components.push_back(stol(words[i]));
                cout << words[i];    
                }           
            }
        
    } // End while

    file.close();

}

// Petropoulakis Panagiotis
