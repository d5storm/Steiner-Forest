#include "Forest_Steiner.hpp"

int main(int argc, char *argv[]) {
    Grafo * test = new Grafo(argv[1]);
    int seed = stoi(argv[2]);
    int uchoaIter = stoi(argv[3]);
    test->solve(seed, uchoaIter);
    
}