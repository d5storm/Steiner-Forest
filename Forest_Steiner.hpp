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
#include "./src/rfw_random.h"


using namespace std;



class Edge
{
    public:
        int id;
        int vertex_a, vertex_b;
        double weight;
        double tempWeight;
        bool usedEdge;
        int appearance;
    Edge(int id, int vertex_a, int vertex_b, double weight){
        this->id = id;
        usedEdge = false;
        this->vertex_a = vertex_a;
        this->vertex_b = vertex_b;
        this->weight = weight;
        this->tempWeight = weight;
        this->appearance = 0;
    }

    void useEdge(){
        this->usedEdge = true;
        this->appearance++;
    }
    void forceUnUseEdge(){ 
        this->appearance = 0;
        this->usedEdge = false;
    }
    void unUseEdge(){
        if(this->appearance == 1){
            this->appearance = 0;
            this->usedEdge = false;
        } else if(this->appearance > 1)
            this->appearance--;
    }
};

class Nugget
{
    public:
        int vertex_a, vertex_b;
        list<Edge*> * pathEdges;

    Nugget(int vertex_a, int vertex_b, list<Edge*> * pathEdges){
        this->vertex_a = vertex_a;
        this->vertex_b = vertex_b;
        this->pathEdges = pathEdges;
    }
};

class Grafo
{
public:
    int V,E;
    // list<int> *adj; // adjacent list
    Grafo(string path); // constructor
    double solvePrim(RFWLocalRandom * random, int seed, int iter);
    double solveLuidi(RFWLocalRandom * random);
    void printGraph();
    Edge* getEdge(int id);
    Edge* getEdge(int vertex_a, int vertex_b);
    vector<vector<int>*> getTerminalGroup(int pos);
    int getSolutionCost();
    bool isFeasible(){
        return detectCicle() && checkTerminalsMeet()?true:false;
    }
private:

    bool detectCicle();
    bool DFS(int start, int father, vector<int> * visited);
    bool checkTerminalsMeet();

    list<Nugget*> * solution;
    vector<vector<int>*> * terminals;
    vector<vector<int>*> * adj;
    vector<vector<int>*> * steinerForest;
    vector<Edge*>* edges;
    list<Edge*>* usedEdges;
    vector<Graph> * treeGraphs;

    bool relocateLocalSearch();


    void pushNugget(int vertex_a, int vertex_b, list<Edge*> * path);
    void insertNugget(int pos, int vertex_a, int vertex_b, list<Edge*> * path);
    Nugget * removeNugget(int pos);
    void createSteinerForestAdj();
    void clearSteinerForestAdj();
    void addEdge(int id, int vertex_a, int vertex_b, double weight);
    void useEdge(int e);
    void unUseEdge(int e);
    void useEdge(Edge * e);
    void unUseEdge(Edge * e);


    vector<int>* addTerminalGroup();
    vector<vector<Edge*>*>* getConnectedComponents();
    void removeCiclesWithPrim(vector<vector<Edge*>*>* components);
    
    void solveByPath(RFWLocalRandom * random);
    list<Edge*> * connectTwoVertexDijkstra(int vertex_source, int vertex_dest, vector<vector<int>*> * matrix);
    int minDistance(vector<int> dist, vector<bool> sptSet);  
    void addToPath(vector<int> parent, int j, list<Edge*>* usedEdgesOnPath);
};

#endif
