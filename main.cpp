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
    int eliteSetSize = stoi(argv[5]);
    int support = stoi(argv[6]);
    double execTime = stod(argv[7]);
    bool DM;
    string dm_string(argv[8]);
    if (dm_string.compare("true") == 0)
        DM = true;
    else
        DM = false;

    Mining* mminer = new Mining(eliteSetSize, support, 30, random);

    bool useIter = false;
    bool useTime = false;
    if(execTime != -1)
        useTime = true;
    else if(algIter != -1)
        useIter = true;

    int bestSol = INT_MAX;
    double totalTime = 0.0;
    int iter = 0;
    Grafo * best = new Grafo(argv[1]);
    cout << argv[1];
    int iterBestFound = -1;
    int totalEdgeLS = 0;

    bool mined = false;
    if (DM && useIter == -1){
        cout << "NUMERO DE ITER NAO INSERIDO" << endl;
        exit(1);
    }
    while(checkTime(useTime, totalTime, execTime) && checkiter(useIter, iter, algIter)){
        // cout << " ITER: " << iter << endl;
        if(DM && (iter == algIter / 2 )){
            // cout << "mining!" << endl;
            // cin.get();
            // mminer->printES();
            mminer->map_file();
            mminer->mine();
            mminer->unmapall_file(best->V);
            // cout << "mined!" << endl;
            // cin.get();
            mined = true;
        }
        bool solToES = false;
        Grafo * testLuidi = new Grafo(argv[1]);
        // cout << totalTime << endl;
        if(!DM || !mined){
            // cout << "solving not mined" << endl;
            totalTime += testLuidi->solveLuidi(random, 0, &totalEdgeLS, alpha, false, NULL);
            // cout << "solved not mined" << endl;
        } else{
            Pattern * p = mminer->getCurrentPattern();
            mminer->nextPattern();
            totalTime += testLuidi->solveLuidi(random, 0, &totalEdgeLS, alpha, true, p->elements);
            // cout << "CREATED WITH PATTERN" << endl;
            // testLuidi->printGraph();
            // cin.get();
        }
        if(!testLuidi->isFeasible()){
            cout << "Luidi UnFeasible!" << endl;
            exit(-1);
        }
        if(DM && mminer->updateES(testLuidi) && !mined){
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
            bestSol = testLuidi->getSolutionCost();
            // delete best;
            best = testLuidi;
            iterBestFound = iter;
        } else{
            if(!solToES){
                // cout << "deleted sol" << endl;
                // delete testLuidi;
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