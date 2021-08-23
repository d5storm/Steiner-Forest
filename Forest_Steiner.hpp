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

    void useEdgeAsPattern(){
        this->usedEdge = true;
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
        vector<bool> * isEdgeIdUsed;

    Nugget(int vertex_a, int vertex_b, list<Edge*> * pathEdges, int totalEdges){
        this->vertex_a = vertex_a;
        this->vertex_b = vertex_b;
        this->pathEdges = pathEdges;
        this->isEdgeIdUsed = new vector<bool>(totalEdges, false);
        list<Edge *>::iterator it = this->pathEdges->begin();
        int e;
        for(e = 0; e < this->pathEdges->size(); it++){
            Edge * edge = *it;
            // cout << "EdgeID: " << edge->id << " isEdgeUsedSize: " << isEdgeIdUsed->size() << endl;
            // cin.get();
            this->isEdgeIdUsed->at(edge->id) = true;
            e++;
        }
    }
};

class Grafo
{
public:
    int V,E;
    // list<int> *adj; // adjacent list
    Grafo(string path); // constructor
    double solvePrim(RFWLocalRandom * random, int seed, int iter);
    double solveLuidi(RFWLocalRandom * random, int perturbation, int * totalEdgeLS, double alpha, bool usePattern, vector<vector<int>*> * elem, bool useTarget, int target);
    void printGraph();
    Edge* getEdge(int id);
    Edge* getEdge(int vertex_a, int vertex_b);
    vector<Edge*> * getEdges() {return edges;};
    vector<vector<int>*> getTerminalGroup(int pos);
    int totalUsedEdges() { return usedEdges->size();}
    int patternSize() { return usedPatternSize; }
    int getSolutionCost();
    bool isFeasible(){
        return detectCicle() && checkTerminalsMeet()?true:false;
    }
private:

    bool detectCicle();
    bool DFS(int start, int father, vector<int> * visited);
    bool checkTerminalsMeet();


    int usedPatternSize = 0;
    list<Nugget*> * solution;
    vector<vector<int>*> * terminals;
    vector<vector<int>*> * adj;
    vector<vector<int>*> * steinerForest;
    vector<vector<int>*> * perturbationAux;
    vector<Edge*>* edges;
    list<Edge*>* usedEdges;
    vector<Graph> * treeGraphs;

    bool relocateLocalSearch();
    bool relocateHardLocalSearch(); // TODO: VERSÃO DO RELOCATE QUE REALMENTE TENTA A "NOVA" ORDEM ESTABELECIDA.
    bool removeEdgeLocalSearch(bool firstImprovement);
    bool reDoNugget(); // TODO: DE TEMPOS EM TEMPOS COMEÇAR DE NOVO A SOLUÇÃO COM A ORDEM ESTABELECIDA.

    void removeAndInsertPerturbation(int percentage, RFWLocalRandom * random);



    void pushNugget(int vertex_a, int vertex_b, list<Edge*> * path, int totalEdges);
    void insertNugget(int pos, int vertex_a, int vertex_b, list<Edge*> * path, int totalEdges);
    Nugget * removeNugget(int pos);
    void createSteinerForestAdj();
    void updatePerturbationAux();
    void clearSteinerForestAdj();
    void addEdge(int id, int vertex_a, int vertex_b, double weight);
    void useEdge(int e);
    void unUseEdge(int e);
    void useEdge(Edge * e);
    void unUseEdge(Edge * e);
    void clearUnusedPatternEdges();


    vector<int>* addTerminalGroup();
    vector<vector<Edge*>*>* getConnectedComponents();
    void removeCiclesWithPrim(vector<vector<Edge*>*>* components);
    
    void solveByPath(RFWLocalRandom * random, bool usePattern, vector<vector<int>*> * elem);
    void GRASP(RFWLocalRandom * random, double alpha);
    void construct(vector<std::pair<int,int>> * pairs, vector<vector<int>*> * adj, RFWLocalRandom * random);
    void constructGrasp(vector<std::pair<int,int>> * pairs, RFWLocalRandom * random, double alpha);
    list<Edge*> * connectTwoVertexDijkstra(int vertex_source, int vertex_dest, vector<vector<int>*> * matrix);
    int minDistance(vector<int> dist, vector<bool> sptSet);  
    void addToPath(vector<int> parent, int j, list<Edge*>* usedEdgesOnPath);
};

#endif
