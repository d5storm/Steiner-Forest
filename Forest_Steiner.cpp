//
//  Forest_Steiner.cpp
//  Steiner_Tree
//
//  Created by sergio junior on 12/11/19.
//  Copyright © 2019 sergio junior. All rights reserved.
//

#include "Forest_Steiner.hpp"

using namespace std;

vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
 
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
 
  return internal;
}

void Grafo::clearUnusedPatternEdges(){
    for(int e = 0; e < edges->size(); e++){
        Edge * edge = edges->at(e);
        if(edge->usedEdge && edge->appearance == 0){
            edge->forceUnUseEdge();
        }
    }
}

Edge * Grafo::getEdge(int vertex_a, int vertex_b){
    // cout << "Looking for Edge with: " << vertex_a << " and " << vertex_b << endl;
    for(int e = 0; e < edges->size(); e++){
        Edge * edge = edges->at(e);
        if(edge->vertex_a == vertex_a && edge->vertex_b == vertex_b)
            return edge;
        if(edge->vertex_b == vertex_a && edge->vertex_a == vertex_b)
            return edge;
    }
    return NULL;
}

Grafo::Grafo(string path){
    std::ifstream file;
    file.open(path, std::ifstream::in);

    string line;

    getline(file, line);

    int nVertex = stoi(split(line, ' ')[1]);
    this->solution = new list<Nugget*>();
    this->V = nVertex; // atribui o número de vértices
    // adj = new list<int>[V]; // cria as listas
    this->adj = new vector<vector<int>*>();
    this->steinerForest = new vector<vector<int>*>();
    this->perturbationAux = new vector<vector<int>*>();
    for(unsigned int i = 0; i < this->V; i++){
        vector<int>* line = new vector<int>();
        vector<int>* lineSteiner = new vector<int>();
        vector<int>* linePerturbation = new vector<int>();
        for(unsigned j = 0; j < V; j++){
            line->push_back(-1);
            lineSteiner->push_back(-1);
            linePerturbation->push_back(-1);
        }
        this->adj->push_back(line);
        this->steinerForest->push_back(lineSteiner);
        this->perturbationAux->push_back(linePerturbation);
    }
    this->terminals = new vector<vector<int>*>();
    this->edges = new vector<Edge*>();
    this->usedEdges = new list<Edge*>();
    this->treeGraphs = new vector<Graph>();
    // cout << "nVertex: " << nVertex << endl;

    // cin.get();
    int edgeCount = 0;
    while (getline(file, line)){
        vector<string> split_line = split(line, ' ');
        string type = split_line[0];
        // cout << "Type: " << type << endl;
        if(type == "E"){
            // cout << "Lendo Aresta" << endl;
            int vertex_a = stoi(split_line[1]);
            int vertex_b = stoi(split_line[2]);
            double weight = stod(split_line[3]);
            // cout << vertex_a << " " << vertex_b << " " << weight << endl;
            // cin.get();
            this->addEdge(edgeCount, vertex_a, vertex_b, weight);
            edgeCount++;
        } else if(type == "S"){
            // cout << "Lendo Terminal" << endl;
            vector<int> * terminalGroup = this->addTerminalGroup();
            // for(unsigned int i = 1; i < split_line.size(); i++){
            //     cout << stoi(split_line[i]) << " ";
            // }
            // cout << endl; 
            // cin.get();
            for(unsigned int i = 1; i < split_line.size(); i++){
                terminalGroup->push_back(stoi(split_line[i]));
            }
        }
    }
    // gFinal->printGraph();
    // cin.get();
    file.close();
}

vector<int>* Grafo::addTerminalGroup(){
    terminals->push_back(new vector<int>());
    return terminals->back();
}


void Grafo::addToPath(vector<int> parent, int j, list<Edge*>* usedEdgesOnPath){
    if(parent[j] < 0) return;
    
    addToPath(parent, parent[j], usedEdgesOnPath);
    Edge * edge = getEdge(j, parent[j]);
    usedEdgesOnPath->push_back(edge);


}

int Grafo::minDistance(vector<int> dist, vector<bool> sptSet) 
{ 
      
    // Initialize min value 
    int min = 99999999, min_index; 
  
    for (int v = 0; v < V; v++) 
        if (sptSet[v] == false && 
                   dist[v] <= min) 
            min = dist[v], min_index = v; 
  
    return min_index; 
} 

list<Edge*> * Grafo::connectTwoVertexDijkstra(int vertex_source, int vertex_dest, vector<vector<int>*> * matrix){
    

    vector<int> dist(V);  
    vector<bool> sptSet(V); 

    vector<int> parent(V); 
  
    for (int i = 0; i < V; i++) 
    { 
        parent[vertex_source] = -1; 
        dist[i] = 99999999; 
        sptSet[i] = false; 
    } 

    dist[vertex_source] = 0; 
  
    // Find shortest path 
    // for all vertices 
    for (int count = 0; count < V - 1; count++) 
    { 
        // Pick the minimum distance 
        // vertex from the set of 
        // vertices not yet processed.  
        // u is always equal to src 
        // in first iteration. 
        int u = minDistance(dist, sptSet); 
  
        // Mark the picked vertex  
        // as processed 
        sptSet[u] = true; 
  
        // Update dist value of the  
        // adjacent vertices of the 
        // picked vertex. 
        for (int v = 0; v < V; v++){ 
  
            // Update dist[v] only if is 
            // not in sptSet, there is 
            // an edge from u to v, and  
            // total weight of path from 
            // src to v through u is smaller 
            // than current value of 
            // dist[v] 
            if (!sptSet[v] && matrix->at(u)->at(v) > -1 && 
                dist[u] + matrix->at(u)->at(v) < dist[v]) 
            { 
                parent[v] = u; 
                dist[v] = dist[u] + matrix->at(u)->at(v); 
            }  
        }
    } 
    
    

    // cout << "parent vector: " << endl;
    // for (int v = 0; v < parent.size(); v++){ 
    //     cout << parent[v] << " ";
    // }
    // cout << endl;
    // cout << "dist vector: " << endl;
    // for (int v = 0; v < parent.size(); v++){ 
    //     cout << dist[v] << " ";
    // }
    // cout << endl; cin.get();
    list<Edge*> * path = new list<Edge*>();
    if(dist[vertex_dest] == 99999999) {cout << "NO PATH from: " << vertex_source << " to: " << vertex_dest << endl; return path;}

    
    addToPath(parent, vertex_dest, path);

    // cout << "Path: " << path->size() << endl;
    // for (int v = 0; v < path->size(); v++){ 
    //     cout << "(" << path->at(v)->vertex_a << "," << path->at(v)->vertex_b << ") ";
    // }
    // cout << endl;
    // cin.get();
    // print the constructed 
    // distance array 
    // printSolution(dist, V, parent); 
    return path;
}

void Grafo::pushNugget(int vertex_a, int vertex_b, list<Edge*> * path, int totalEdges){
    Nugget * newInsertion = new Nugget(vertex_a, vertex_b, path, totalEdges);
    this->solution->push_back(newInsertion);
}
void Grafo::insertNugget(int pos, int vertex_a, int vertex_b, list<Edge*> * path, int totalEdges){
    Nugget * newInsertion = new Nugget(vertex_a, vertex_b, path, totalEdges);
    list<Nugget *>::iterator it = this->solution->begin();
    advance(it, pos);
    this->solution->insert(it, newInsertion);
}
Nugget * Grafo::removeNugget(int pos){
    list<Nugget *>::iterator it = this->solution->begin();
    advance(it, pos);
    Nugget * n = *it;
    this->solution->erase(it);
    return n;
}

void Grafo::construct(vector<std::pair<int,int>> * pairs, vector<vector<int>*> * adj, RFWLocalRandom * random){
    while(pairs->size() > 0){
        // cout << "Pairs Left: " << terminalToUse->size() << endl;
        int pairPos = random->GetRand() % pairs->size();
        int pivot = pairs->at(pairPos).first;
        int dest = pairs->at(pairPos).second;

        list<Edge*> * path = connectTwoVertexDijkstra(pivot, dest, this->adj);
        // cout << "Pair Added: <" << pivot << "," << dest << ">" << endl;
        pairs->erase(pairs->begin() + pairPos);
        // for (int v = 0; v < path->size(); v++){ 
        //     cout << "(" << path->at(v)->vertex_a << "," << path->at(v)->vertex_b << ") ";
        // }
        // cin.get();
        list<Edge *>::iterator it = path->begin();
        int e = 0;
        for(e = 0; e < path->size(); it++){
            Edge * edge;
            edge = *it;
            useEdge(edge);
            int vertex_a = edge->vertex_a;
            int vertex_b = edge->vertex_b;
            adj->at(vertex_a)->at(vertex_b) = 0;
            adj->at(vertex_b)->at(vertex_a) = 0;
            e++;
        }
        this->pushNugget(pivot, dest, path, this->edges->size());
        // printGraph();
        // cin.get();
    
    }
}

void Grafo::constructGrasp(vector<std::pair<int,int>> * pairs, RFWLocalRandom * random, double alpha){
    vector<vector<int>*> * currentState = new vector<vector<int>*>();
    for(int i = 0; i < this->V; i++){
        vector<int>* line = new vector<int>(this->V);
        for(int y = 0; y < this->V; y++){
            line->at(y) = adj->at(i)->at(y);
        }
        currentState->push_back(line);
    }

    vector<bool> edgesPresent(this->edges->size(), false);
    while(pairs->size() > 0){
        for(int e = 0; e < edges->size(); e++){
            int vertex_a = edges->at(e)->vertex_a;
            int vertex_b = edges->at(e)->vertex_b;
            if(edges->at(e)->usedEdge){
                currentState->at(vertex_a)->at(vertex_b) = 0;
                currentState->at(vertex_b)->at(vertex_a) = 0;
            }else{
                currentState->at(vertex_a)->at(vertex_b) = edges->at(e)->weight;
                currentState->at(vertex_b)->at(vertex_a) = edges->at(e)->weight;
            }
        }
        // cout << "Available pairs: " << endl;
        // for(int p = 0; p < pairs->size(); p++){
        //     cout << "<" << pairs->at(p).first << "," << pairs->at(p).second << "> ";
        // }   
        // cout<<endl;
        vector<list<Edge*> *> * orderedPath = new vector<list<Edge*> *>();
        vector<std::pair<int,int>> * orderedPairs = new vector<std::pair<int,int>>();
        vector<double> * orderedCost = new vector<double>();
        vector<int> * orderedIndex = new vector<int>();
        for(int p = 0; p < pairs->size(); p++){
            int vertex_a = pairs->at(p).first;
            int vertex_b = pairs->at(p).second;
            list<Edge*> * new_path = this->connectTwoVertexDijkstra(vertex_a, vertex_b, currentState);
            // cout << "Pair: " << vertex_a << "," << vertex_b << endl;
            // cout << "PathSize: " << new_path->size() << endl;
            list<Edge *>::iterator it3 = new_path->begin();
            int e = 0;
            // for(e = 0; e < new_path->size(); it3++){
            //     Edge * edge = *it3;
            //     cout << "<" << edge->vertex_a << "," << edge->vertex_b << ">";
            //     e++;
            // }
            // cout << endl;
            
            double newCost = 0;
            it3 = new_path->begin();
            for(e = 0; e < new_path->size(); it3++){
                Edge * edge = *it3;
                // cout << "<" << edge->vertex_a << "," << edge->vertex_b << "> Cost: " << edge->weight << endl;
                if(!edge->usedEdge){
                    newCost += edge->weight;
                }
                e++;
            }
            // cout << "PathCost: " << newCost << endl;
            // cin.get();
            
            bool inserted = false;
            if(orderedPath->size() == 0){
                orderedPath->push_back(new_path);
                orderedCost->push_back(newCost);
                orderedIndex->push_back(p);
                orderedPairs->push_back(pairs->at(p));
            } else{
                for(int e = 0; e < orderedPath->size(); e++){
                    if(newCost < orderedCost->at(e)){
                        inserted = true;
                        orderedCost->insert(orderedCost->begin() + e, newCost);
                        orderedPath->insert(orderedPath->begin() + e, new_path);
                        orderedIndex->insert(orderedIndex->begin() + e, p);
                        orderedPairs->insert(orderedPairs->begin() + e, pairs->at(p));
                        break;
                    }
                }
                if(!inserted){
                    orderedCost->push_back(newCost);
                    orderedPath->push_back(new_path);
                    orderedIndex->push_back(p);
                    orderedPairs->push_back(pairs->at(p));
                }
            }
        }

        int LRC = (int)ceil(alpha * orderedPath->size());

        // cout << "LRC: " << LRC << endl;
        int pos = random->GetRand() % LRC;
        // cout << "pos: " << pos << endl;

        // cout << "ChosenPosCost: " << orderedCost->at(pos) << endl;

        list<Edge*> * path = orderedPath->at(pos);

        std::pair<int,int> pairPos = orderedPairs->at(pos);
        // cout << "ChosenPair: <" << pairPos.first << "," << pairPos.second  << ">" << endl;

        pairs->erase(pairs->begin() + orderedIndex->at(pos));

        list<Edge *>::iterator it = path->begin();
        int e = 0;
        for(e = 0; e < path->size(); it++){
            Edge * edge;
            edge = *it;
            useEdge(edge);
            int vertex_a = edge->vertex_a;
            int vertex_b = edge->vertex_b;
            adj->at(vertex_a)->at(vertex_b) = 0;
            adj->at(vertex_b)->at(vertex_a) = 0;
            e++;
        }
        this->pushNugget(pairPos.first, pairPos.second, path, this->edges->size());
        
        
        // cin.get();
        delete orderedPath;
        delete orderedPairs;
        delete orderedCost;
        delete orderedIndex;
    }

    // this->printGraph();
    // cin.get();
}

void Grafo::GRASP(RFWLocalRandom * random, double alpha){
    vector<std::pair<int,int>> * terminalToUse = new vector<std::pair<int,int>>();
    for(int g = 0; g < terminals->size(); g++){
        vector<int> availableVertex;
        for(int t = 0; t < terminals->at(g)->size(); t++){
            availableVertex.push_back(terminals->at(g)->at(t));    
        }
        while(availableVertex.size() > 1){
            int pivotPos = random->GetRand() % availableVertex.size();
            int pivot = availableVertex[pivotPos];
            availableVertex.erase(availableVertex.begin() + pivotPos);
            int destPos = random->GetRand() % availableVertex.size();
            int dest = availableVertex[destPos];
            std::pair <int, int> newPair(pivot, dest);
            // cout << "Pair Created: <" << newPair.first << "," << newPair.second << ">" << endl;
            terminalToUse->push_back(newPair);
        }
    }
    // cout << "Created Pairs" << endl;
    // for(int i = 0; i < terminalToUse->size(); i++){
    //     cout << "<" << terminalToUse->at(i).first << "," << terminalToUse->at(i).second << "> ";
    // }
    // cout << endl;
    // cin.get();
    constructGrasp(terminalToUse, random, alpha);    
}

void Grafo::solveByPath(RFWLocalRandom * random, bool usePattern, vector<vector<int>*>* elem){

    if(usePattern){
        this->usedPatternSize = elem->size();
        // cout << "using pattern" << endl;
        for(int i = 0; i < elem->size(); i++){
            int vertex_a = elem->at(i)->at(0);
            int vertex_b = elem->at(i)->at(1);
            Edge * e = getEdge(vertex_a, vertex_b);
            e->useEdgeAsPattern();
            adj->at(vertex_a)->at(vertex_b) = 0;
            adj->at(vertex_b)->at(vertex_a) = 0;
        }
        // printGraph();
        // cin.get();
    }
    
    vector<std::pair<int,int>> * terminalToUse = new vector<std::pair<int,int>>();
    for(int g = 0; g < terminals->size(); g++){
        vector<int> availableVertex;
        for(int t = 0; t < terminals->at(g)->size(); t++){
            availableVertex.push_back(terminals->at(g)->at(t));    
        }
        while(availableVertex.size() > 1){
            int pivotPos = random->GetRand() % availableVertex.size();
            int pivot = availableVertex[pivotPos];
            availableVertex.erase(availableVertex.begin() + pivotPos);
            int destPos = random->GetRand() % availableVertex.size();
            int dest = availableVertex[destPos];
            std::pair <int, int> newPair(pivot, dest);
            // cout << "Pair Created: <" << newPair.first << "," << newPair.second << ">" << endl;
            terminalToUse->push_back(newPair);
        }
    }
    construct(terminalToUse, this->adj, random);
    if(usePattern)
        clearUnusedPatternEdges();

    for(int e = 0; e < edges->size(); e++){
        int vertex_a = edges->at(e)->vertex_a;
        int vertex_b = edges->at(e)->vertex_b;
        adj->at(vertex_a)->at(vertex_b) = edges->at(e)->weight;
        adj->at(vertex_b)->at(vertex_a) = edges->at(e)->weight;
    }
}

void Grafo::removeAndInsertPerturbation(int percentage, RFWLocalRandom * random){

    
    int totalMoves = ceil(float(this->solution->size()) * (float)percentage/100.0);
    vector<std::pair<int,int>> removedPairs(totalMoves);
    int move = 0;
    while(totalMoves > move){
        int chosenIndex = random->GetRand() % solution->size();
        // cout << "Chosen Index: " << chosenIndex << endl;
        Nugget * removed = this->removeNugget(chosenIndex);
        list<Edge *>::iterator it = removed->pathEdges->begin();
        int e;
        for(e = 0; e < removed->pathEdges->size(); it++){
            Edge * edge = *it;
            unUseEdge(edge);
            if(!edge->usedEdge){
                this->perturbationAux->at(edge->vertex_a)->at(edge->vertex_b) = edge->weight;
                this->perturbationAux->at(edge->vertex_b)->at(edge->vertex_a) = edge->weight;
            }
            e++;
        }
        delete removed->pathEdges;
        // cout << "Removed: " << removed->vertex_a << " " << removed->vertex_b << endl;
        pair<int,int> pair(removed->vertex_a, removed->vertex_b);
        delete removed;
        removedPairs[move] = pair;
        move++;
    }
    

    // vector<vector<int>*> pAdj(this->V, new vector<int>(this->V, -1));

    // for(unsigned int i = 0; i < this->V; i++){
    //     for(unsigned j = i + 1; j < this->V; j++){
    //         Edge * e = this->getEdge(i, j);
    //         if(e != NULL){
    //             if(e->usedEdge){
    //                 pAdj[i]->at(j) = 0;
    //                 pAdj[j]->at(i) = 0;
    //             }
    //             else{
    //                 pAdj[i]->at(j) = e->weight;
    //                 pAdj[j]->at(i) = e->weight;
    //             }   
    //         }
    //     }
    // }

    construct(&removedPairs, this->perturbationAux, random);


}

bool Grafo::removeEdgeLocalSearch(bool firstImprovement){
    // cout << "Starting Edge Local Search..." << endl;
    // auto totalTime = std::chrono::system_clock::now();
    // auto start = std::chrono::system_clock::now();

    // double step1,step2,step3 = 0.0;
    // this->printGraph();
    double bestCost = this->getSolutionCost();
    bool improved = false;

    vector<Nugget *> * bestAlteredNuggets = new vector<Nugget *>();
    vector<list<Edge*>*> * bestNewPaths = new vector<list<Edge*>*>();

    vector<vector<int>*> * currentState = new vector<vector<int>*>();
    for(int i = 0; i < this->V; i++){
        vector<int>* line = new vector<int>(this->V, -1);
        // for(int j = 0; j < this->V; j++){
        //     line->push_back(-1);
        // }
        currentState->push_back(line);
    }
    // auto end = std::chrono::system_clock::now();
    // std::chrono::duration<double> elapsed_seconds = end-start;
    // cout << "Time to Build Current State: " << (double)elapsed_seconds.count() << endl; 
    // start = std::chrono::system_clock::now();
    this->usedEdges->sort([](Edge * lhs, Edge * rhs) {return lhs->appearance < rhs->appearance;});
    list<Edge *>::iterator it = this->usedEdges->begin();
    int i = 0;
    for(i = 0; i < this->usedEdges->size(); it++){
        double newCost = 0.0;
        Edge * edgeToRemove = *it;
        // cout << "Edge Appearence: " << edgeToRemove->appearance << endl;
        // cin.get();
        // continue;
        // cout << "Removing Edge: " << edgeToRemove->id << " cost: " << edgeToRemove->weight << endl;
        // auto intern = std::chrono::system_clock::now();
        vector<int> removedEdge(this->edges->size(), 0);
        vector<bool> edgesPresent(this->edges->size(), false);

        vector<Nugget *> * alteredNuggets = new vector<Nugget *>();
        vector<list<Edge*>*> * allNewPaths = new vector<list<Edge*>*>();

        vector<pair<int,int>> pairs;
        list<Nugget *>::iterator it2 = this->solution->begin();
        int e;
        for(e = 0; e < this->solution->size(); it2++){
            Nugget * n = *it2;
            // cout << "EdgePresent: " << n->isEdgeIdUsed->at(edgeToRemove->id) << endl;
            // cin.get();
            if(!n->isEdgeIdUsed->at(edgeToRemove->id)){
                e++;
                continue;
            }
            alteredNuggets->push_back(n);
            pair<int,int> nPair(n->vertex_a, n->vertex_b);
            pairs.push_back(nPair);
            for(int p = 0; p < n->isEdgeIdUsed->size(); p++){
                if(n->isEdgeIdUsed->at(p))
                    removedEdge[p]++;
            }
            e++;
        }
        // end = std::chrono::system_clock::now();
        // elapsed_seconds = end-intern;
        // step1 += (double)elapsed_seconds.count();
        // cout << "Created Pairs" << endl;
        // intern = std::chrono::system_clock::now();
        for(e = 0; e < this->edges->size();e++){
            Edge * edge = this->edges->at(e);
            if(edge->id == edgeToRemove->id) continue;
            if(edge->usedEdge){
                newCost += edge->weight;
                edgesPresent[edge->id] = true;
                currentState->at(edge->vertex_a)->at(edge->vertex_b) = 0;
                currentState->at(edge->vertex_b)->at(edge->vertex_a) = 0;
                if(removedEdge[edge->id] == edge->appearance){
                    currentState->at(edge->vertex_a)->at(edge->vertex_b) = edge->weight;
                    currentState->at(edge->vertex_b)->at(edge->vertex_a) = edge->weight;
                    edgesPresent[edge->id] = false;
                    newCost -= edge->weight;
                }
            } else{
                currentState->at(edge->vertex_a)->at(edge->vertex_b) = edge->weight;
                currentState->at(edge->vertex_b)->at(edge->vertex_a) = edge->weight;
            }
            
        }
        currentState->at(edgeToRemove->vertex_a)->at(edgeToRemove->vertex_b) = 10000;
        currentState->at(edgeToRemove->vertex_b)->at(edgeToRemove->vertex_a) = 10000;
        // end = std::chrono::system_clock::now();
        // elapsed_seconds = end-intern;
        // step2 += (double)elapsed_seconds.count();
        // intern = std::chrono::system_clock::now();
        // cout << "CurrentState:" << endl;
        // for(int s = 0; s < this->V; s++){
        //     for(int s1 = 0; s1 < this->V; s1++){
        //         cout << currentState->at(s)->at(s1) << " ";
        //     }
        //     cout << endl;
        // }

        // cout << "Updated CurrentState: " << newCost << endl;
        // cin.get();
        for(int p = 0; p < pairs.size(); p++){
            int vertex_a = pairs.at(p).first;
            int vertex_b = pairs.at(p).second;
            list<Edge*> * new_path = this->connectTwoVertexDijkstra(vertex_a, vertex_b, currentState);
            allNewPaths->push_back(new_path);
            list<Edge *>::iterator it3 = new_path->begin();
            int e = 0;
            for(e = 0; e < new_path->size(); it3++){
                Edge * edge = *it3;
                currentState->at(edge->vertex_a)->at(edge->vertex_b) = 0;
                currentState->at(edge->vertex_b)->at(edge->vertex_a) = 0;
                if(edgesPresent[edge->id] == false){
                    newCost += edge->weight;
                    edgesPresent[edge->id] = true;
                }
                e++;
            }
            if(newCost > bestCost){
                break;
            }
        }
        // end = std::chrono::system_clock::now();
        // elapsed_seconds = end-intern;
        // step3 += (double)elapsed_seconds.count();

        // cout << "cost after EdgeRemoval: " << newCost << endl;
        // cout << "CurrentState:" << endl;
        // for(int s = 0; s < this->V; s++){
        //     for(int s1 = 0; s1 < this->V; s1++){
        //         cout << currentState->at(s)->at(s1) << " ";
        //     }
        //     cout << endl;
        // }
        // cin.get();
        if(newCost < bestCost){
            improved = true;
            bestCost = newCost;
            delete bestNewPaths;
            delete bestAlteredNuggets;
            bestNewPaths = allNewPaths;
            bestAlteredNuggets = alteredNuggets;
            if(firstImprovement){
                i++;
                break;
            }

        } else{
            delete allNewPaths;
            delete alteredNuggets;
        }
        i++;
    }
    // end = std::chrono::system_clock::now();
    // elapsed_seconds = end-start;
    // cout << "Time to Check All UsedEdges: " << (double)elapsed_seconds.count() << endl;
    // cout << "Step1: " << step1 << endl;
    // cout << "Step2: " << step2 << endl; 
    // cout << "Step3: " << step3 << endl; 
    
    delete currentState;
    if(improved){
        // start = std::chrono::system_clock::now();
        // cout << "finished edge local search with improvement" << endl;
        // cout << "Solution Before:" << endl;
        // printGraph();
        // cout << "****************" << endl;
        for(int n = 0; n < bestAlteredNuggets->size(); n++){
            list<Edge*> * oldPath = bestAlteredNuggets->at(n)->pathEdges;
            list<Edge *>::iterator it = oldPath->begin();
            for(int p = 0; p < oldPath->size(); it++){
                Edge * edge = *it;
                unUseEdge(edge);
                p++;
            }
            delete bestAlteredNuggets->at(n)->isEdgeIdUsed;
            bestAlteredNuggets->at(n)->isEdgeIdUsed = new vector<bool>(this->edges->size(), false);
            it = bestNewPaths->at(n)->begin();
            for(int p = 0; p < bestNewPaths->at(n)->size(); it++){
                Edge * edge = *it;
                bestAlteredNuggets->at(n)->isEdgeIdUsed->at(edge->id) = true;
                useEdge(edge);
                p++;
            }

            delete bestAlteredNuggets->at(n)->pathEdges;
            bestAlteredNuggets->at(n)->pathEdges = bestNewPaths->at(n);
        }
        // createSteinerForestAdj();
        // cout << "Solution After: " << endl;
        // printGraph();
        // cin.get();
        if(!this->isFeasible()){
            cout << "Luidi UnFeasible! LOCALSEARCH EDGE" << endl;
            cin.get();
            exit(-1);
        }
        // printGraph();
        // cin.get();
        // end = std::chrono::system_clock::now();
        // elapsed_seconds = end-start;
        // cout << "Time to Apply Improvement: " << (double)elapsed_seconds.count() << endl; 
        // end = std::chrono::system_clock::now();
        // elapsed_seconds = end-totalTime;
        // cout << "Time to Finish LS: " << (double)elapsed_seconds.count() << endl;
        return true;
    }
    // end = std::chrono::system_clock::now();
    // elapsed_seconds = end-totalTime;
    // cout << "Time to Finish LS: " << (double)elapsed_seconds.count() << endl; 
    // cout << "finished edge local search without improvement" << endl;
    return false;
}

bool Grafo::relocateLocalSearch(){
    double bestCost = this->getSolutionCost();
    // printGraph();
    // cout << "startingCost: " << bestCost << endl;
    list<Edge*> * bestPath = new list<Edge*>();
    int bestI;
    bool improved = false;
    // cin.get();
    vector<vector<int>*> * currentState = new vector<vector<int>*>();
    for(int i = 0; i < this->V; i++){
        vector<int>* line = new vector<int>(this->V, -1);
        // for(int j = 0; j < this->V; j++){
        //     line->push_back(-1);
        // }
        currentState->push_back(line);
    }

    for(int e = 0; e < edges->size(); e++){
        int vertex_a = edges->at(e)->vertex_a;
        int vertex_b = edges->at(e)->vertex_b;
        currentState->at(vertex_a)->at(vertex_b) = edges->at(e)->weight;
        currentState->at(vertex_b)->at(vertex_a) = edges->at(e)->weight;
    }

    // cout << "Printing currentState..." << endl;
    // for(int i = 0; i < this->V; i++){
    //     for(int j = 0; j < this->V; j++){
    //         cout << currentState->at(i)->at(j) << " ";
    //     }
    //     cout << endl;
    // }
    // cin.get();
    list<Nugget *>::iterator it = this->solution->begin();
    int i = 0;
    for(i = 0; i < this->solution->size() - 1; it++){
        // cout << "i: " << i << endl;
        Nugget * nugget_i = *it;
        int vertex_a = nugget_i->vertex_a;
        int vertex_b = nugget_i->vertex_b;
        double newCost = 0.0;
        vector<int> edgesPresent(this->edges->size(), 0);
        list<Nugget *>::iterator it2 = this->solution->begin();
        int j = 0;
        for(j = 0; j < this->solution->size(); it2++){
            // cout << "j: " << j << endl;
            if(j == i) {
                j++;
                continue;
            }
            Nugget * nugget_j = *it2;
            list<Edge *> * path = nugget_j->pathEdges;
            list<Edge *>::iterator it3 = path->begin();
            int e = 0;
            for( e = 0; e < path->size(); it3++){
                Edge * edge = *it3;
                int vertex_a = edge->vertex_a;
                int vertex_b = edge->vertex_b;
                currentState->at(vertex_a)->at(vertex_b) = 0;
                currentState->at(vertex_b)->at(vertex_a) = 0;
                if(edgesPresent[edge->id] == 0){
                    newCost += edge->weight;
                    edgesPresent[edge->id] = 1;
                }
                e++;
            }
            j++;
        }


        // cout << "cost after removal: " << newCost << endl;

        // cout << "Printing currentState..." << endl;
        // for(int i = 0; i < this->V; i++){
        //     for(int j = 0; j < this->V; j++){
        //         cout << currentState->at(i)->at(j) << " ";
        //     }
        //     cout << endl;
        // }
        // cin.get();
        // cout << "i: " << i << endl;
        list<Edge*> * new_path = this->connectTwoVertexDijkstra(vertex_a, vertex_b, currentState);
        list<Edge *>::iterator it3 = new_path->begin();
        int e = 0;
        for(e = 0; e < new_path->size(); it3++){
            Edge * edge = *it3;
            if(edgesPresent[edge->id] == 0){
                newCost += edge->weight;
                edgesPresent[edge->id] = 1;
            }
            e++;
        }

        // cout << "cost after reinsert: " << newCost << endl;
        if(newCost < bestCost){
            improved = true;
            bestCost = newCost;
            delete bestPath;
            bestPath = new_path;
            bestI = i;

        } else{
            delete new_path;
        }
        

        for(int e = 0; e < edges->size(); e++){
            int vertex_a = edges->at(e)->vertex_a;
            int vertex_b = edges->at(e)->vertex_b;
            currentState->at(vertex_a)->at(vertex_b) = edges->at(e)->weight;
            currentState->at(vertex_b)->at(vertex_a) = edges->at(e)->weight;
        }
        i++;
    }
    delete currentState;
    // cout << "saiu" << endl;
    if(improved){
        // printGraph();
        // cout << "^^^^^^" << endl;
        list<Nugget *>::iterator it = this->solution->begin();
        advance(it, bestI);
        Nugget * nugget = *it;
        list<Edge *>::iterator it2 = nugget->pathEdges->begin();
        int e = 0;
        for(e = 0; e < nugget->pathEdges->size(); it2++){
            Edge * edge = *it2;
            this->unUseEdge(edge->id);
            nugget->isEdgeIdUsed->at(edge->id) = false;
            e++;
        }
        // cout << "Melhorou!" << endl;
        // cout << "BestI: " << bestI << endl;
        // cout << "Suposto melhor custo: " << bestCost << endl;
        // for(int e = 0; e < bestPath->size(); e++){
        //     Edge * edge = bestPath->at(e);
        //     cout << "a: " << edge->vertex_a << " " << "b: " << edge->vertex_b << endl;
        // }
        // cin.get();
        it2 = bestPath->begin();
        e = 0;
        for(e = 0; e < bestPath->size(); it2++){
            Edge * edge = *it2;
            this->useEdge(edge);
            nugget->isEdgeIdUsed->at(edge->id) = true;
            e++;
        }
        delete nugget->pathEdges;
        nugget->pathEdges = bestPath;

        if(!this->isFeasible()){
            cout << "Luidi UnFeasible! LOCALSEARCH" << endl;
            exit(-1);
        }
        // printGraph();
        // cin.get();
        return true;
    }
    return false;
}

double Grafo::solveLuidi(RFWLocalRandom * random, int perturbation, int * totalEdgeLS, double alpha, bool usePattern, vector<vector<int>*> * elem, bool useTarget, int target){
    auto start = std::chrono::system_clock::now();
    solveByPath(random, usePattern, elem);
    if (useTarget && this->getSolutionCost() <= target){
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        createSteinerForestAdj();
        cout << "construtivo" << " ";
        return (double)elapsed_seconds.count();
    }
    
    createSteinerForestAdj();
    
    int pCost;
    bool BLimproved = true;
    bool tryEdge = false;
    int BLITers = 0;
    while(BLimproved){
        BLimproved = removeEdgeLocalSearch(true);
        BLITers++;
        if (useTarget && this->getSolutionCost() <= target){
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end-start;
            createSteinerForestAdj();
            cout << "buscalocal_" << BLITers << " ";
            return (double)elapsed_seconds.count();
        }
        
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    createSteinerForestAdj();

    return (double)elapsed_seconds.count();
}

void Grafo::updatePerturbationAux(){
    for(int i = 0; i < this->edges->size(); i++){
        Edge * e = this->edges->at(i);
        if(e->usedEdge){
            this->perturbationAux->at(e->vertex_a)->at(e->vertex_b) = 0;
            this->perturbationAux->at(e->vertex_b)->at(e->vertex_a) = 0;
        }else{
            this->perturbationAux->at(e->vertex_a)->at(e->vertex_b) = e->weight;
            this->perturbationAux->at(e->vertex_b)->at(e->vertex_a) = e->weight;
        }
    }
}

void Grafo::createSteinerForestAdj(){
    for(int i = 0; i < this->edges->size(); i++){
        Edge * e = this->edges->at(i);
        if(e->usedEdge){
            this->steinerForest->at(e->vertex_a)->at(e->vertex_b) = e->weight;
            this->steinerForest->at(e->vertex_b)->at(e->vertex_a) = e->weight;
        }else{
            this->steinerForest->at(e->vertex_a)->at(e->vertex_b) = -1;
            this->steinerForest->at(e->vertex_b)->at(e->vertex_a) = -1;
        }
    }
    // for(unsigned int i = 0; i < this->V; i++){
    //     for(unsigned j = 0; j < V; j++){
    //         cout << steinerForest->at(i)->at(j) << " " ;
    //     }
    //     cout << endl;
    // }
    // cin.get();
}

void Grafo::clearSteinerForestAdj(){
    for(unsigned int i = 0; i < this->V; i++){
        for(unsigned j = 0; j < V; j++){
            this->steinerForest->at(i)->at(j) = -1;
        }
    }
}

void Grafo::printGraph(){
    // cout << "Adj Matrix:" << endl;
    // for(unsigned int i = 0; i < adj->size(); i++){
    //     for(unsigned int j = 0; j < adj->at(i)->size(); j++){
    //         int multiplier = 1;
    //         // for(unsigned int e = 0; e < edges->size(); e++){
    //         //     if((edges->at(e)->vertex_a == i && edges->at(e)->vertex_b == j) || (edges->at(e)->vertex_a == j && edges->at(e)->vertex_b == i) && !edges->at(e)->usedEdge){
    //         //         multiplier = -1;
    //         //         break;
    //         //     }
    //         // }
    //         cout << adj->at(i)->at(j)*multiplier << " ";
    //     }
    //     cout << endl;
    // }

    cout << "Used Edges:" << endl;
    for(unsigned int i = 0; i < edges->size(); i++){
        if(edges->at(i)->usedEdge)
            cout << "(" << edges->at(i)->vertex_a << "," << edges->at(i)->vertex_b << ")[" << edges->at(i)->appearance << "] "; 
    }
    cout << endl;

    // cout << "Group of Terminals:" << endl;
    // for(unsigned int i = 0; i < terminals->size(); i++){
    //     cout << "Group " << i << ": ";
    //     for(unsigned int j = 0; j < terminals->at(i)->size(); j++){
    //         cout << terminals->at(i)->at(j) << " ";
    //     }
    //     cout << endl;
    // }

    cout << "Solution:" << endl;
    list<Nugget *>::iterator it = solution->begin();
    int i = 0;
    for(i = 0; i < solution->size(); it++){
        Nugget * n = *it;
        cout << "(" << n->vertex_a << ", " << n->vertex_b << ") : ";
        list<Edge *>::iterator it2 = n->pathEdges->begin();
        int j = 0;
        for(j = 0; j < n->pathEdges->size(); it2++){
            Edge * e = *it2;
            cout << "[" << e->vertex_a << "," << e->vertex_b << "] ->";
            j++;
        }
        cout << endl;
        i++;
    }
    cout << "Cost: " << getSolutionCost() << endl;

    cout << "\n******************GRAPHVIZ******************" << endl;
    vector<string> colors = {"red", "blue", "yellow", "purple", "brown", "green", "burlywood", "darkgoldenrod3", "turquoise1", "orangered4"};

    for(unsigned int i = 0; i < edges->size(); i++){
        if(edges->at(i)->usedEdge){
            cout << edges->at(i)->vertex_a << " -- " << edges->at(i)->vertex_b << " [label=\"" <<  edges->at(i)->weight << "\"]"  << ";" << endl; 
        }
    }
    cout << "Colors: " << colors.size() << " T: " << terminals->size() << endl;
    for(unsigned int t = 0; t < terminals->size(); t++){
        for(unsigned int t1 = 0; t1 < terminals->at(t)->size(); t1++){
            cout << terminals->at(t)->at(t1) << " [color=" << colors[t] << "];" << endl;
        }
    }

    cout << endl;
}

void Grafo::useEdge(int e){
    if(!edges->at(e)->usedEdge){
        usedEdges->push_back(edges->at(e));
    }
    edges->at(e)->useEdge();
}

void Grafo::useEdge(Edge * e){
    if(!e->usedEdge || (e->usedEdge && e->appearance == 0)){
        usedEdges->push_back(e);
    }
    e->useEdge();
}

void Grafo::unUseEdge(int e){
    edges->at(e)->unUseEdge();
    if(!edges->at(e)->usedEdge){
        list<Edge *>::iterator it = usedEdges->begin();
        int i = 0;
        for(i = 0; i < usedEdges->size(); it++){
            Edge * edge = *it;
            if(edge->id == edges->at(e)->id){
                usedEdges->erase(it);
                return;
            }
            i++;
        }
    }
}

void Grafo::unUseEdge(Edge * e){
    e->unUseEdge();
    if(!e->usedEdge){
        list<Edge *>::iterator it = usedEdges->begin();
        int i = 0;
        for(i = 0; i < usedEdges->size(); it++){
            Edge * edge = *it;
            if(edge == e){
                usedEdges->erase(it);
                return;
            }
        }
    }
}

vector<vector<Edge*>*>* Grafo::getConnectedComponents(){
    
    vector <vector <Edge*>*>* components = new vector<vector<Edge*>*>();
    
    vector<Edge*>* group;
    
    vector<Edge*>* availableEdges = new vector<Edge*>();
    
    // for(int e = 0; e < usedEdges->size(); e++){
    //     availableEdges->push_back(usedEdges->at(e));
    // }


    while(availableEdges->size() != 0){
        // cout << "TotalUsedEdges: " << availableEdges->size() << endl;
        // cin.get();
        group = new vector<Edge*>();
        vector<int> vertex;
        group->push_back(availableEdges->back());
        
        vertex.push_back(availableEdges->back()->vertex_a);
        vertex.push_back(availableEdges->back()->vertex_b);
        // cout << "Pushed: " << availableEdges->back().vertex_a << "," << availableEdges->back().vertex_b << endl;
        // cin.get();
        availableEdges->pop_back();
        // cout << "TotalUsedEdges: " << availableEdges->size() << endl;
        // cin.get();
        int e = 0;
        while(e < availableEdges->size()){
            Edge * edge = availableEdges->at(e);
            // cout << "Checking Edge: ("<<edge.vertex_a<<","<<edge.vertex_b<<")"<<endl;
            // cin.get();
            bool vertex_a_in = false;
            bool vertex_b_in = false;
            for(int v = 0; v < vertex.size(); v++){
                if(edge->vertex_a == vertex[v]){
                    // cout << "Vertex_a (" << edge.vertex_a << ") in!" << endl;
                    vertex_a_in = true;
                }
                if(edge->vertex_b == vertex[v]){
                    // cout << "Vertex_b (" << edge.vertex_b << ") in!" << endl;
                    vertex_b_in = true;
                }
            }
            if(vertex_a_in && !vertex_b_in){
                vertex.push_back(edge->vertex_b);
            }
            if(vertex_b_in && !vertex_a_in){
                vertex.push_back(edge->vertex_a);
            }
            
            if(vertex_a_in || vertex_b_in){
                // cout << "Added Edge: ("<<edge.vertex_a<<","<<edge.vertex_b<<")"<<endl;
                group->push_back(edge);
                availableEdges->erase(availableEdges->begin() + e);
                e = 0;
            }else{
                e++;
                // cout << "No Use Edge: ("<<edge.vertex_a<<","<<edge.vertex_b<<")"<<endl;
            }
            // cout << "Vertex size: " << vertex.size() << endl;
            // cout << "Group size: " << group->size() << endl;
            // cout << "AvailableEdges: " << availableEdges->size() << endl;
            // cin.get();

        }
        components->push_back(group);
        // cout << "Added connected component!" << endl;
        // for(int e = 0; e < group->size(); e++){
        //     cout << "(" << group->at(e)->vertex_a << "," << group->at(e)->vertex_b << ")";
        // }
        // cout << endl;
        // cout << "TotalUsedEdges: " << availableEdges->size() << endl;
        // cin.get();
    }

    // if(components->size() > 1){
    //     printGraph();
    //     for(int c = 0; c < components->size(); c++){
    //         vector<Edge*> * group = components->at(c);
    //         cout << "Group_"<< c << ": ";
    //         for(int e = 0; e < group->size(); e++){
    //             cout << "(" << group->at(e)->vertex_a << "," << group->at(e)->vertex_b << ")";
    //         }
    //         cout << endl;
    //     }
    //     cin.get();
    // }
    
    return components;
}

int minKey(int key[], bool mstSet[], int V)  
{  
    // Initialize min value  
    int min = INT_MAX, min_index;  
  
    for (int v = 0; v < V; v++)  
        if (mstSet[v] == false && key[v] < min)  
            min = key[v], min_index = v;  
  
    return min_index;  
}  
   
void printMST(vector<int> parent, vector<vector<int>> graph, int V)  
{  
    cout<<"Edge \tWeight\n";  
    for (int i = 1; i < V; i++)  
        cout<<parent[i]<<" - "<<i<<" \t"<<graph[i][parent[i]]<<" \n";  
}  
  
vector<int> primMST(vector<vector<int>> graph, int V)  
{  
    // Array to store constructed MST  
    vector<int> parent(V);  
      
    // Key values used to pick minimum weight edge in cut  
    int key[V];  
      
    // To represent set of vertices not yet included in MST  
    bool mstSet[V];  
  
    // Initialize all keys as INFINITE  
    for (int i = 0; i < V; i++)  
        key[i] = INT_MAX, mstSet[i] = false;  
  
    // Always include first 1st vertex in MST.  
    // Make key 0 so that this vertex is picked as first vertex.  
    key[0] = 0;  
    parent[0] = -1; // First node is always root of MST  
  
    // The MST will have V vertices  
    for (int count = 0; count < V - 1; count++) 
    {  
        // Pick the minimum key vertex from the  
        // set of vertices not yet included in MST  
        int u = minKey(key, mstSet, V);  
  
        // Add the picked vertex to the MST Set  
        mstSet[u] = true;  
  
        // Update key value and parent index of  
        // the adjacent vertices of the picked vertex.  
        // Consider only those vertices which are not  
        // yet included in MST  
        for (int v = 0; v < V; v++)  
  
            // graph[u][v] is non zero only for adjacent vertices of m  
            // mstSet[v] is false for vertices not yet included in MST  
            // Update the key only if graph[u][v] is smaller than key[v]  
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])  
                parent[v] = u, key[v] = graph[u][v];  
    }  
  
    // print the constructed MST  
    // printMST(parent, graph, V);  
    return parent;
}  

vector<vector<int>> createPrimGraph(vector<int> vertex, vector<Edge*>* e){

    vector<vector<int>> primGraph;
    

    // cout << "Present Vertex: ";
    // for(int i = 0; i < vertex.size(); i++){
    //     cout << vertex[i] << " ";
    // }
    // cout << endl;
    // cin.get();
    for(int i = 0; i < vertex.size(); i++){
        int originalVertex_a = vertex[i];
        vector<int> line;
        for(int j = 0; j< vertex.size(); j++){
            int originalVertex_b = vertex[j];
            if(i == j){ 
                line.push_back(0); 
            } else{
                bool added = false;
                for(int x = 0; x < e->size(); x++){
                    if((e->at(x)->vertex_a == originalVertex_a) && (e->at(x)->vertex_b == originalVertex_b) || (e->at(x)->vertex_a == originalVertex_b) && (e->at(x)->vertex_b == originalVertex_a)){
                        line.push_back(e->at(x)->weight);
                        added = true;
                    }
                }
                if(!added)
                    line.push_back(0);
            }

        }
        primGraph.push_back(line);
    }
    return primGraph;
}

void Grafo::removeCiclesWithPrim(vector<vector<Edge*>*>* components){
    // cout << "Total Components: " << components->size() << endl;
    for(int c = 0; c < components->size(); c++){
        vector<Edge*>* e = components->at(c);
        vector<int> vertex;

        for(int i = 0; i < e->size(); i++){
            int a = e->at(i)->vertex_a;
            int b = e->at(i)->vertex_b;

            bool a_in = false;
            bool b_in = false;

            for(int v = 0; v < vertex.size(); v++){
                if(vertex[v] == a) a_in = true;
                if(vertex[v] == b) b_in = true;
            }

            if(!a_in) vertex.push_back(a);
            if(!b_in) vertex.push_back(b);
        }
        vector<vector<int>> primGraph = createPrimGraph(vertex, components->at(c));

        // cout << "Created Graph: " << endl;
        // for(int i = 0; i < primGraph.size(); i++){
        //     for(int j = 0; j < primGraph[i].size(); j++){
        //         cout << primGraph[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cin.get();
        vector<int> parents = primMST(primGraph, primGraph.size());
        
        for(int edge = 0; edge < e->size(); edge++){
            // cout << "Edge: (" << e->at(edge)->vertex_a << "," << e->at(edge)->vertex_b << ")" << endl;
            // cin.get();
            bool trimmed = true;            
            for(int v = 0; v < parents.size(); v++){
                int vertex_a = vertex[v];
                int vertex_b = vertex[parents[v]];
                // cout << "Vertex_a: " << vertex_a << " Vertex_b" << vertex_b << endl;
                if((e->at(edge)->vertex_a == vertex_a && e->at(edge)->vertex_b == vertex_b) || (e->at(edge)->vertex_a == vertex_b && e->at(edge)->vertex_b == vertex_a)){
                    trimmed = false;
                    break;
                }
            }

            if(trimmed){
                unUseEdge(e->at(edge));
            }
        }
        // printGraph();
        // cin.get();
    }
}

int Grafo::getSolutionCost(){
    int cost = 0;
    
    list<Edge *>::iterator it = usedEdges->begin();
    int i = 0;
    for(i = 0; i < usedEdges->size(); it++){
        Edge * edge = *it;
        cost += edge->weight;
        i++;
    }

    return cost;
}

    
void Grafo::addEdge(int id, int vertex_a, int vertex_b, double weight){
    id = edges->size();
    Edge * newEdge = new Edge(id, vertex_a, vertex_b, weight);
    edges->push_back(newEdge);
    adj->at(vertex_a)->at(vertex_b) = weight;
    adj->at(vertex_b)->at(vertex_a) = weight;
    perturbationAux->at(vertex_a)->at(vertex_b) = weight;
    perturbationAux->at(vertex_b)->at(vertex_a) = weight;
}

bool Grafo::checkTerminalsMeet(){
     for(int t = 0; t < terminals->size(); t++){
         for(int v1 = 0; v1 < terminals->at(t)->size(); v1++){
             for(int v2 = v1+1; v2 < terminals->at(t)->size(); v2++){
                list<Edge*> * path = connectTwoVertexDijkstra(terminals->at(t)->at(v1), terminals->at(t)->at(v2), this->steinerForest);
                if(path->size() == 0)
                    return false;
             }
         }
     }
     return true;
}

bool Grafo::detectCicle(){
    vector<int> visited(V,0);
    for(int i = 0; i < V; i++){
        if(visited[i] == 0)
            if(DFS(i, -1, &visited)){
                return false;
            }
    }
    return true;
}

bool Grafo::DFS(int start, int father, vector<int> * visited){
    visited->at(start) = 1;
    for(int i = 0; i < V; i++){
        if(i == start) continue;
        if(i == father) continue;
        if(steinerForest->at(start)->at(i) < 0) continue;
        if(visited->at(i) > 0) return true;
        DFS(i, start, visited);
    }
    return false;
}