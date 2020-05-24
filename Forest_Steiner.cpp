//
//  Forest_Steiner.cpp
//  Steiner_Tree
//
//  Created by sergio junior on 12/11/19.
//  Copyright © 2019 sergio junior. All rights reserved.
//

#include "Forest_Steiner.hpp"

vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
 
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
 
  return internal;
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

    this->V = nVertex; // atribui o número de vértices
    // adj = new list<int>[V]; // cria as listas
    this->adj = new vector<vector<int>*>();
    for(unsigned int i = 0; i < this->V; i++){
        vector<int>* line = new vector<int>();
        for(unsigned j = 0; j < V; j++){
            line->push_back(0);
        }
        this->adj->push_back(line);
    }
    this->terminals = new vector<vector<int>*>();
    this->edges = new vector<Edge*>();
    this->usedEdges = new vector<Edge*>();
    this->treeGraphs = new vector<Graph>();
    // cout << "nVertex: " << nVertex << endl;

    // cin.get();

    while (getline(file, line)){
        vector<string> split_line = split(line, ' ');
        string type = split_line[0];
        // cout << "Type: " << type << endl;
        if(type == "E"){
            // cout << "Lendo Aresta" << endl;
            int vertex_a = stoi(split_line[1]);
            int vertex_b = stoi(split_line[2]);
            double weight = stod(split_line[3]);
            this->addEdge(vertex_a, vertex_b, weight);
        } else if(type == "S"){
            // cout << "Lendo Terminal" << endl;
            vector<int> * terminalGroup = this->addTerminalGroup();
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

void Grafo::solveSteinerTrees(int seed, int iter){
    for(unsigned int st = 0; st < treeGraphs->size(); st++){
        // cout << "Graph Memory: " << treeGraphs->at(st) << endl;
        // cout << "Solving this... " << endl;
        // for(int e = 0; e <= treeGraphs->at(st).EdgeCount(); e++){
        //     cout << treeGraphs->at(st).GetFirstEndpoint(e) << " " << treeGraphs->at(st).GetSecondEndpoint(e) << " " << treeGraphs->at(st).GetCost(e) << endl;
        // }
        vector<vector<int>> usedEdges = SPGSolver::Solve(treeGraphs->at(st), seed, iter);
        // cout << "Solved Steiner Tree: " << st << endl;
        // cin.get();
        for(unsigned int e = 0; e < usedEdges.size(); e++){
            int vertex_a = usedEdges.at(e).at(0);
            int vertex_b = usedEdges.at(e).at(1);
            // cout << "vertex_a: " << vertex_a << " vertex_b: " << vertex_b << endl;
            // cin.get();
            for(unsigned int a = 0; a < edges->size(); a++){
                // cout << "Edge: " << a << " vertex_a: " << edges->at(a).vertex_a << " vertex_b: " << edges->at(a).vertex_b << endl;
                int my_vertex_a = edges->at(a)->vertex_a + 1;
                int my_vertex_b = edges->at(a)->vertex_b + 1;
                // cout << "Edge: " << a << " vertex_a: " << my_vertex_a << " vertex_b: " << my_vertex_b << endl;
                // cin.get();
                if((my_vertex_a == vertex_a && my_vertex_b == vertex_b) || (my_vertex_a == vertex_b && my_vertex_b == vertex_a)){
                    // cout << "Usou a edge!" << endl;
                    useEdge(a);
                    // edges->at(a).useEdge();
                    break;
                }
            }
        }
        // cout << "Added Steiner edges:" << endl;
        // printGraph();
        // cin.get();
    }
}

void Grafo::addToPath(vector<int> parent, int j, vector<Edge*>* usedEdgesOnPath){
    if (parent[j] >= 0){
        addToPath(parent, parent[j], usedEdgesOnPath); 
        Edge * edge = getEdge(j, parent[j]);
        usedEdgesOnPath->push_back(edge);
    }

}


int Grafo::minDistance(vector<int> dist, vector<bool> sptSet) 
{ 
      
    // Initialize min value 
    int min = INT_MAX, min_index; 
  
    for (int v = 0; v < V; v++) 
        if (sptSet[v] == false && 
                   dist[v] <= min) 
            min = dist[v], min_index = v; 
  
    return min_index; 
} 

vector<Edge*> * Grafo::connectTwoVertexDijkstra(int vertex_source, int vertex_dest){
    vector<int> dist(V);  
    vector<bool> sptSet(V); 

    vector<int> parent(V); 
  
    for (int i = 0; i < V; i++) 
    { 
        parent[0] = -1; 
        dist[i] = INT_MAX; 
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
            if (!sptSet[v] && adj->at(u)->at(v) && 
                dist[u] + adj->at(u)->at(v) < dist[v]) 
            { 
                parent[v] = u; 
                dist[v] = dist[u] + adj->at(u)->at(v); 
            }  
        }
    } 
    
    vector<Edge*> * path = new vector<Edge*>();
    addToPath(parent, vertex_dest, path);

    // cout << "Path: " << path->size() << endl;
    // for (int v = 0; v < path->size(); v++){ 
    //     cout << "(" << path->at(v)->vertex_a << "," << path->at(v)->vertex_b << ") ";
    // }
    // cout << endl;
    // print the constructed 
    // distance array 
    // printSolution(dist, V, parent); 
    return path;
}

void solveLuidi(int seed, int iter){

}

void Grafo::createSteinerTrees(){
    int nEdges = edges->size();
    int nVertex = adj->size();

    // printGraph();
    // cin.get();
    for(int t=0; t < terminals->size(); t++){
        Graph steinerTree;
        int tdeclared = 0;
        // cout << "nVertex: " << nVertex << " nEdges: " << nEdges << endl;

        steinerTree.coord.SetMaxId(nVertex);
        steinerTree.SetVertices(nVertex);

        steinerTree.SetEdges(nEdges);

        for(int e = 0; e < edges->size(); e++){
            // cout << "Adding edge: " << edges->at(e).vertex_a + 1<< " " << edges->at(e).vertex_b + 1<< " "  << (EdgeCost)edges->at(e).weight << endl;
            // cin.get();
            int uchoa_vertex_a = edges->at(e)->vertex_a + 1;
            int uchoa_vertex_b = edges->at(e)->vertex_b + 1;
            double cost = edges->at(e)->weight;
            // cout << "Adding edge: " << uchoa_vertex_a<< " " << uchoa_vertex_b << " "  << (EdgeCost)cost << endl;
            steinerTree.AddEdge(uchoa_vertex_a, uchoa_vertex_b, (EdgeCost)cost);
        }

        for(int s=0; s < terminals->at(t)->size(); s++){
            // cout << "Adding terminal: " << terminals->at(t)->at(s) + 1<< endl;
            // cin.get();
            tdeclared++;
            int terminal = terminals->at(t)->at(s) + 1;
            steinerTree.MakeTerminal(terminal);
        }
        // if (tdeclared != steinerTree.TerminalCount()) cout << "invalid number of terminals" << endl; cin.get();

        steinerTree.Commit();
        // cout << "Edges added to Uchoa: " << steinerTree.EdgeCount()<< endl;
        // for(int e = 0; e <= steinerTree.EdgeCount(); e++){
        //     cout << steinerTree.GetFirstEndpoint(e) << " " << steinerTree.GetSecondEndpoint(e) << " " << steinerTree.GetCost(e) << endl;
        // }
        // cout << "PRINTED!" << endl;
        // cin.get();
        // steinerTree.Commit();

        // cout << "Steiner Uchoa: " << endl;
        // steinerTree->OutputGraph(stdout);
        // cin.get();
        treeGraphs->push_back(steinerTree);
    }
    // cin.get();
}

void Grafo::printGraph(){
    cout << "Adj Matrix:" << endl;
    for(unsigned int i = 0; i < adj->size(); i++){
        for(unsigned int j = 0; j < adj->at(i)->size(); j++){
            int multiplier = 1;
            // for(unsigned int e = 0; e < edges->size(); e++){
            //     if((edges->at(e)->vertex_a == i && edges->at(e)->vertex_b == j) || (edges->at(e)->vertex_a == j && edges->at(e)->vertex_b == i) && !edges->at(e)->usedEdge){
            //         multiplier = -1;
            //         break;
            //     }
            // }
            cout << adj->at(i)->at(j)*multiplier << " ";
        }
        cout << endl;
    }

    cout << "Used Edges:" << endl;
    for(unsigned int i = 0; i < edges->size(); i++){
        if(edges->at(i)->usedEdge)
            cout << "(" << edges->at(i)->vertex_a << "," << edges->at(i)->vertex_b << ") "; 
    }
    cout << endl;

    cout << "Group of Terminals:" << endl;
    for(unsigned int i = 0; i < terminals->size(); i++){
        cout << "Group " << i << ": ";
        for(unsigned int j = 0; j < terminals->at(i)->size(); j++){
            cout << terminals->at(i)->at(j) << " ";
        }
        cout << endl;
    }

    cout << "Cost: " << getSolutionCost() << endl;
}

void Grafo::useEdge(int e){
    if(!edges->at(e)->usedEdge){
        edges->at(e)->useEdge();
        usedEdges->push_back(edges->at(e));
    }
}

void Grafo::useEdge(Edge * e){
    if(!e->usedEdge){
        e->unUseEdge();
        usedEdges->push_back(e);
    }
}

void Grafo::unUseEdge(int e){
    edges->at(e)->unUseEdge();
    for(int i = 0; i < usedEdges->size(); i++){
        if((usedEdges->at(i)->vertex_a == edges->at(e)->vertex_a) && (usedEdges->at(i)->vertex_b == edges->at(e)->vertex_b)){
            usedEdges->erase(usedEdges->begin() + i);
            return;
        }
    }
}

void Grafo::unUseEdge(Edge * e){
    e->unUseEdge();
    for(int i = 0; i < usedEdges->size(); i++){
        if(usedEdges->at(i) == e){
            usedEdges->erase(usedEdges->begin() + i);
            return;
        }
    }
}

vector<vector<Edge*>*>* Grafo::getConnectedComponents(){
    
    vector <vector <Edge*>*>* components = new vector<vector<Edge*>*>();
    
    vector<Edge*>* group;
    
    vector<Edge*>* availableEdges = new vector<Edge*>();
    
    for(int e = 0; e < usedEdges->size(); e++){
        availableEdges->push_back(usedEdges->at(e));
    }


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
    // cin.get();
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
  
// A utility function to print the  
// constructed MST stored in parent[]  
void printMST(vector<int> parent, vector<vector<int>> graph, int V)  
{  
    cout<<"Edge \tWeight\n";  
    for (int i = 1; i < V; i++)  
        cout<<parent[i]<<" - "<<i<<" \t"<<graph[i][parent[i]]<<" \n";  
}  
  
// Function to construct and print MST for  
// a graph represented using adjacency  
// matrix representation  
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
    for(int i = 0; i < usedEdges->size(); i++){
        cost += usedEdges->at(i)->weight;
    }

    return cost;
}

double Grafo::solvePrim(int seed, int iter){
    auto start = std::chrono::system_clock::now();
    createSteinerTrees();
    solveSteinerTrees(seed, iter);
    // printGraph();
    // cin.get();
    vector<vector<Edge*>*>* components = getConnectedComponents();
    removeCiclesWithPrim(components);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    printGraph();
    cin.get();
    return (double)elapsed_seconds.count();
    
}
    
void Grafo::addEdge(int vertex_a, int vertex_b, double weight){ 
    Edge * newEdge = new Edge(vertex_a, vertex_b, weight);
    edges->push_back(newEdge);
    adj->at(vertex_a)->at(vertex_b) = weight;
    adj->at(vertex_b)->at(vertex_a) = weight;
}
