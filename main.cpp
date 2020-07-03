#include "Forest_Steiner.hpp"

int main(int argc, char *argv[]) {

    // cout << "leu" << endl;
    int seed = stoi(argv[2]);
    RFWRandom::randomize(seed);
    RFWLocalRandom * random = new RFWLocalRandom(seed);
    int algIter = stoi(argv[3]);
    int uchoaIter = stoi(argv[4]);
    // cout << "leu" << endl;
    double execTime = stod(argv[5]);

    if(execTime == -1)
        execTime = INT64_MAX;
    if(algIter == -1)
        algIter = INT64_MAX;
    // cout << "ExecTime: " << execTime << endl;
    
    // cout << "algiter: " << algIter << " uchoaITer: " << uchoaIter << endl;
    int bestSol = 99999999999999;
    double totalTime = 0.0;
    int iter = 0;
    while(totalTime <= execTime && iter < algIter){
        // cout << totalTime << endl;
        Grafo * testLuidi = new Grafo(argv[1]);
        // Grafo * testSergio = new Grafo(argv[1]);
        // totalTime += testSergio->solvePrim(random, seed, uchoaIter);
        totalTime += testLuidi->solveLuidi(random);
        if(!testLuidi->isFeasible()){
            cout << "Luidi UnFeasible!" << endl;
            exit(-1);
        }
        // if(!testSergio->isFeasible()){
        //     cout << "Sergio UnFeasible!" << endl;
        //     exit(-2);
        // }
        // cout << "New Solution Cost: " << testLuidi->getSolutionCost() << endl;
        // cin.get();
        testLuidi->printGraph();
        cin.get();
        if(testLuidi->getSolutionCost() < bestSol){
            bestSol = testLuidi->getSolutionCost();
        }
        // if(testSergio->getSolutionCost() < bestSol){
        //     bestSol = testSergio->getSolutionCost();
        // }
        // delete testSergio;
        delete testLuidi;
        iter++;
    }
    
    // testLuidi->printGraph();
    
    
    // cout << "CostLuidi: " << testLuidi->getSolutionCost() << " LuidiTime: " << execTimeLuidi << " Seed: " << seed << endl;
    cout << argv[1] << " " << bestSol << " " << totalTime << endl;
    // cin.get();
    // cout << test->getSolutionCost() << " " << execTime << endl;
    return 1;
    
}