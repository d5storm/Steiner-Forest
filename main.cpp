#include "Forest_Steiner.hpp"
#include "Mining.h"

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
    
    double alpha = stoi(argv[3]) / 10.0;
    int algIter = stoi(argv[4]);
    int uchoaIter = stoi(argv[5]);
    int perturbation = stoi(argv[6]);
    double execTime = stod(argv[7]);

    Mining* mminer = new Mining(10, 8, 30, random);

    bool useIter = true;
    bool useTime = true;
    if(execTime == -1)
        useTime = false;
    else if(algIter == -1)
        useIter = false;

    int bestSol = INT_MAX;
    double totalTime = 0.0;
    int iter = 0;
    Grafo * best = new Grafo(argv[1]);
    cout << argv[1];
    int iterBestFound = -1;
    int totalEdgeLS = 0;

    bool mined = false;
    while(checkTime(useTime, totalTime, execTime) && checkiter(useIter, iter, algIter)){
        if(iter == algIter / 2){
            // mminer->printES();
            mminer->map_file();
            mminer->mine();
            mminer->unmapall_file(best->V);
            // cin.get();
            mined = true;
        }
        bool solToES = false;
        // cout << totalTime << endl;
        Grafo * testLuidi = new Grafo(argv[1]);
        if(!mined){
            totalTime += testLuidi->solveLuidi(random, perturbation, &totalEdgeLS, alpha, false, NULL);
        } else{
            Pattern * p = mminer->getCurrentPattern();
            mminer->nextPattern();
            totalTime += testLuidi->solveLuidi(random, perturbation, &totalEdgeLS, alpha, true, p->elements);
            // cout << "CREATED WITH PATTERN" << endl;
            // testLuidi->printGraph();
            // cin.get();
        }
        if(!testLuidi->isFeasible()){
            cout << "Luidi UnFeasible!" << endl;
            exit(-1);
        }
        if(mminer->updateES(testLuidi) && !mined){
            solToES = true;
            // cout << "ATUALIZOU CE" << endl;
            // mminer->printES();
            // cin.get();
				//cout << "Atualizou CE (size: "<<  mminer->getSizeCE() << " worstCost: "<< mminer->getWorstCostES()+numPtsRot <<  " ) - Iteracao: "<< iterCong << " " << temperatura << " " << numOrigPts-melhorSolucao->numRotSob << endl;
				
		} else{
            // cout << "NAO ATUALIZOU CE" << endl;
            // cin.get();
        }
        if(testLuidi->getSolutionCost() < bestSol){
            // if(testLuidi->getSolutionCost() == 1579.0){
            //     testLuidi->printGraph();
            //     cin.get();
            // }
            bestSol = testLuidi->getSolutionCost();
            delete best;
            best = testLuidi;
            iterBestFound = iter;
        } else{
            if(!solToES){
                delete testLuidi;
            }
        }
        // cin.get();
        iter++;
    }
    
    // best->printGraph();
    
    // cout << "EdgeLS: " << totalEdgeLS << endl;
    cout << " " << iterBestFound << " " << bestSol << " " << totalTime << endl;
    // cin.get();
    return 1;
    
}