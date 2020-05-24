#include "Forest_Steiner.hpp"

int main(int argc, char *argv[]) {

    Grafo * test = new Grafo(argv[1]);
    int seed = stoi(argv[2]);
    int uchoaIter = stoi(argv[3]);
    // double execTime = test->solvePrim(seed, uchoaIter);
    double execTime = test->solveLuidi(seed, uchoaIter);
    cout << "Cost: " << test->getSolutionCost() << " Time: " << execTime << " Seed: " << seed << endl;
    // cout << test->getSolutionCost() << " " << execTime << endl;
    
}