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
    if (support > eliteSetSize){
        support = eliteSetSize;
    }
    double execTime = stod(argv[7]);
    bool DM;
    bool stableDM;
    string dm_string(argv[8]);
    string dm_stable_string(argv[9]);
    int gamma = -1;
    if (dm_string.compare("true") == 0)
        DM = true;
    else
        DM = false;
    if (dm_stable_string.compare("true") == 0){
        gamma = stoi(argv[10]);
        stableDM = true;
    }
    else
        stableDM = false;

    int target = stoi(argv[11]);
    bool useTarget = false;
    if (target > 0) {
        useTarget = true;
        // cout << "Use Target: " << target << endl;
    }

    // if(stableDM)
    //     cout << "StableDM!" << " Gamma: " << gamma << endl;
    // exit(1);
    Mining* mminer = new Mining(eliteSetSize, support, gamma, random);
    bool useIter = false;
    bool useTime = false;
    if(execTime != -1 && !useTarget)
        useTime = true;
    if(algIter != -1)
        useIter = true;

    int bestSol = INT_MAX;

    int bestSolSize = 0;
    int bestSolPatternSize = 0;
    double totalTime = 0.0;
    int avgSolSize = 0;
    int avgPatternSize = 0;
    int iter = 0;
    Grafo * best = NULL;


    int iterBestFound = -1;
    int totalEdgeLS = 0;
    bool mined = false;
    int miningIters = 0;
    if (DM && useIter == -1){
        cout << "NUMERO DE ITER NAO INSERIDO" << endl;
        exit(1);
    }

    // cout << argv[1] << endl;
    // cout << stableDM << endl;
    // cout << DM << endl;
    // exit(1);
    while(checkTime(useTime, totalTime, execTime) && checkiter(useIter, iter, algIter)){
        // cout << iter;
        if(DM && !stableDM && (iter == algIter / 2 )){
            mminer->map_file();
            mminer->mine();
            mminer->unmapall_file(best->V);
            mined = true;
        } else if(DM && stableDM){
            // cout << "MINERACAO POR ESTABILIDADE" << endl;
            // if(mminer->isStableES()){
            //     cout << "ES IS STABLE!" << endl;
            //     if(mminer->EShasChanged()){
            //         cout << "ES HAS CHANGED!" << endl;
            //     } else{
            //         cout << "ES HAS NOT CHANGED!" << endl;
            //     }
            // } else {
            //     cout << "ES IS NOT STABLE!" << endl;
            // }
            if(mminer->isStableES() && mminer->EShasChanged()){
                // cout << "MINING ESTAVEL!" << endl;
                // cin.get();
                mminer->map_file();
                mminer->mine();
                mminer->unmapall_file(best->V);
                mined = true;
                // cout << " 1 ";
            }
            // else
                // cout << " 0 ";
        } else{
            // cout << "deu ruim" << endl;

        }
        bool solToES = false;
        // cout << "lendo grafo" << endl;
        Grafo * testLuidi = new Grafo(argv[1]);
        // cout << testLuidi->V << " " << testLuidi->E << " " << testLuidi->getTotalTerminals() << endl;
        // return -1;
        // cout << "solving luidi" << endl;
        if(!DM || !mined){
            // cout << "!DM || !mined" << endl;
            totalTime += testLuidi->solveLuidi(random, 0, &totalEdgeLS, alpha, false, NULL, useTarget, target);
        } else{
            miningIters += 1;
            Pattern * p = mminer->getCurrentPattern();
            mminer->nextPattern();
            totalTime += testLuidi->solveLuidi(random, 0, &totalEdgeLS, alpha, true, p->elements, useTarget, target);
        }
        if(!testLuidi->isFeasible()){
            cout << "Luidi UnFeasible!" << endl;
            exit(-1);
        }
        // cout << "resolveu!" << endl;
        if(DM){
            if(!mined || (mined && stableDM)){
               bool updated = mminer->updateES(testLuidi);
                if(!updated)
                    delete testLuidi;
                    // cout << "ES UPDATED!" << endl;
                // else
                //     cout << "ES NOT UPDATED!" << endl;
            }
		}
        if(useTarget && testLuidi->getSolutionCost() <= target){
            // cout << iter << " " << totalTime << " " << testLuidi->getSolutionCost() << " " << target << " " << endl;
            // cin.get();
            return 0;
        }
        if(testLuidi->getSolutionCost() < bestSol){
            bestSolSize = testLuidi->totalUsedEdges();
            bestSolPatternSize = testLuidi->patternSize();
            bestSol = testLuidi->getSolutionCost();
            best = testLuidi;
            iterBestFound = iter;
        }
        iter++;
        // cout << "iter somado!" << endl;
        avgPatternSize += testLuidi->patternSize();
        avgSolSize += testLuidi->totalUsedEdges();
        // cout << "SolSize: " << testLuidi->totalUsedEdges() << endl;
        // cout << "0" << " " << iter << " " << totalTime << " " << bestSol << endl;
    }

    // For pattern extraction
    cout << bestSol << endl;
    mminer->map_file();
    mminer->mine();
    mminer->unmapall_file(best->V);
    mminer->printParsedPatterns(random);

    // For Usual Result!
    // cout << bestSol << " " << totalTime << " " << mminer->getBiggestPattern() << " " << mminer->getSmallestPattern() << endl;

    // For Target Reaching!
    // cout << iter << " " << totalTime << " " << best->getSolutionCost() << " " << target << " " << endl;

    // For Irace!
    // cout << bestSol << endl;
    return 0;

}