//
//  Forest_Steiner.hpp
//  Steiner_Tree
//
//  Created by sergio junior on 12/11/19.
//  Copyright © 2019 sergio junior. All rights reserved.
//

#ifndef Read_Instance_hpp
#define Read_Instance_hpp
#include<iostream>
#include <list>
#include <limits.h>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <list>
#include <algorithm> // função find
#include <stack> // pilha para usar na DFS
#include <dirent.h>
#include <sstream>
#include <sstream>
#include <ostream>
#include "Forest_Steiner.hpp"


Grafo* create_steiner_forest(string path){
    std::ifstream file;
    file.open(path, std::ifstream::in);

    string line;

    getline(file, line);

    int nVertex = stoi(split(line, ' ')[1]);

    // cout << "nVertex: " << nVertex << endl;

    // cin.get();

    Grafo * gFinal = new Grafo(nVertex);

    while (getline(file, line)){
        vector<string> split_line = split(line, ' ');
        string type = split_line[0];
        // cout << "Type: " << type << endl;
        if(type == "E"){
            // cout << "Lendo Aresta" << endl;
            int vertex_a = stoi(split_line[1]);
            int vertex_b = stoi(split_line[2]);
            double weight = stod(split_line[3]);
            gFinal->addEdge(vertex_a, vertex_b, weight);
        } else if(type == "S"){
            // cout << "Lendo Terminal" << endl;
            vector<int> * terminalGroup = gFinal->addTerminalGroup();
            for(unsigned int i = 1; i < split_line.size(); i++){
                terminalGroup->push_back(stoi(split_line[i]));
            }
        }
    }
    // gFinal->printGraph();
    // cin.get();
    file.close();

    return gFinal;
}

  
#endif /* Forest_Steiner_hpp */
