#include "Forest_Steiner.hpp"

bool checkTime(bool useTime, double currentTime, double maxTime){
    if(!useTime)
        return true;
    if(currentTime <= maxTime){
        return true;
    }
    return false;
}

bool checkiter(bool useIter, double currentIter, double maxIter){
    if(!useIter)
        return true;
    if(currentIter < maxIter){
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {

    int seed = stoi(argv[2]);
    RFWRandom::randomize(seed);
    RFWLocalRandom * random = new RFWLocalRandom(seed);
    int algIter = stoi(argv[3]);
    int uchoaIter = stoi(argv[4]);
    double execTime = stod(argv[5]);
    bool useIter = true;
    bool useTime = true;
    if(execTime == -1)
        useTime = false;
    else if(algIter == -1)
        useIter = false;

    int bestSol = 99999999999999;
    double totalTime = 0.0;
    int iter = 0;
    Grafo * best = new Grafo(argv[1]);
    cout << argv[1];
    int iterBestFound = -1;
    while(checkTime(useTime, totalTime, execTime) && checkiter(useIter, iter, algIter)){
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
        // testLuidi->printGraph();
        // cin.get();
        if(testLuidi->getSolutionCost() < bestSol){
            bestSol = testLuidi->getSolutionCost();
            delete best;
            best = testLuidi;
            iterBestFound = iter;
        } else{
            delete testLuidi;
        }
        // if(testSergio->getSolutionCost() < bestSol){
        //     bestSol = testSergio->getSolutionCost();
        // }
        // delete testSergio;
        iter++;
    }
    
    // best->printGraph();
    
    
    // cout << "CostLuidi: " << testLuidi->getSolutionCost() << " LuidiTime: " << execTimeLuidi << " Seed: " << seed << endl;
    cout << " " << iterBestFound << " " << bestSol << " " << totalTime << endl;
    // cin.get();
    // cout << test->getSolutionCost() << " " << execTime << endl;
    return 1;
    
}