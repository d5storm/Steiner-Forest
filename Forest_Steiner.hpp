//
//  Forest_Steiner.hpp
//  Steiner_Tree
//
//  Created by sergio junior on 12/11/19.
//  Copyright © 2019 sergio junior. All rights reserved.
//

#ifndef Forest_Steiner_hpp
#define Forest_Steiner_hpp
#include <string>
#include <vector>
#include <iostream>
#include <limits.h>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <list>
#include <algorithm>
#include <stack>
#include <dirent.h>
#include <ostream>
#include <chrono>
#include <queue> 
#include "./src/spgsolver.h"
#include "./src/graph.h"


using namespace std;


class Edge
{
    public:
        int vertex_a, vertex_b;
        double weight;
        bool usedEdge;
    Edge(int vertex_a, int vertex_b, double weight){
        usedEdge = false;
        this->vertex_a = vertex_a;
        this->vertex_b = vertex_b;
        this->weight = weight;
    }

    void useEdge(){this->usedEdge = true;}
    void unUseEdge(){this->usedEdge = false;}
};

class Grafo
{
public:
    int V,E;
    // list<int> *adj; // adjacent list
    Grafo(string path); // constructor
    double solvePrim(int seed, int iter);
    double solveLuidi(int seed, int iter);
    void printGraph();
    Edge* getEdge(int id);
    Edge* getEdge(int vertex_a, int vertex_b);
    vector<vector<int>*> getTerminalGroup(int pos);
    int getSolutionCost();
    bool isFeasible();

private:
    vector<vector<int>*> * terminals;
    vector<vector<int>*> * adj;
    vector<Edge*>* edges;
    vector<Edge*>* usedEdges;
    vector<Graph> * treeGraphs;
    void detectCicle();
    void checkTerminalsMeet();
    void addEdge(int vertex_a, int vertex_b, double weight);
    void useEdge(int e);
    void unUseEdge(int e);
    void useEdge(Edge * e);
    void unUseEdge(Edge * e);
    void solveSteinerTrees(int seed, int iter);

    void createSteinerTrees();

    vector<int>* addTerminalGroup();
    vector<vector<Edge*>*>* getConnectedComponents();
    void removeCiclesWithPrim(vector<vector<Edge*>*>* components);
    
    vector<Edge*> * connectTwoVertexDijkstra(int vertex_source, int vertex_dest);
    int minDistance(vector<int> dist, vector<bool> sptSet);  
    void addToPath(vector<int> parent, int j, vector<Edge*>* usedEdgesOnPath);
};

#endif
