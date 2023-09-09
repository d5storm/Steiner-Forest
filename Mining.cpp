#include "Mining.h"
#include "Forest_Steiner.hpp"

using namespace std;


Mining::Mining(int _sizeES, int _suporte, int _gamma, RFWLocalRandom * random){
	currentPattern = nPatterns = sizeES = 0;
	maxSizeES = _sizeES;
	min_sup_orig = min_sup = _suporte;
	gamma = _gamma;
	worstCostES = -999999999;
	this->random = random;
	worstCostPos = -1;
	listOfPatterns = NULL;
	mined = eschanged = false;
	iterWNC = numberMine = 0;
	ES = new vector<Grafo*>(_sizeES);
}

Mining::~Mining(){
	for(int i = 0; i < sizeES; i++)
		delete (ES->at(i));
	delete(ES);
	for(int i = 0; i < nPatterns; i++){
		//listOfPatterns[i]->~Pattern();
		delete (listOfPatterns->at(i));
	}
	delete(listOfPatterns);
}

void Mining::mine(int nMaxPatterns){

	if(listOfPatterns == NULL){
		listOfPatterns = new vector<Pattern*>(nMaxPatterns);
	}

	// New patterns to be mined
	nPatterns = 0;

	ostringstream buffer;
	buffer.str("");
/*
   Exemplo de utilização:
   	   fpmax_hnmp <semente> <id_arq_tmp> <banco de dados> <tam. do banco> <suporte minimo> <qtd de padroes> <arq. saida>
*/
	buffer
			<< "miner/fpmax_hnmp "
			<< "1 "
			<< (random->GetRand() % 100)
			<< " miner/bd.txt "
			<<  sizeES
			<< " " << min_sup
			<< " " << nMaxPatterns
			<< " miner/padroes.txt" ;
	int s = system(buffer.str().c_str());
	if(s != 0){ cout << "ERROR: function system cannot be performed." << endl; exit(1);}
	buffer.str("");
	iterWNC = 0;
	numberMine++;
	eschanged = false;
	mined = true;
}

// Mapeia soluções elite e printa em um arquivo.
void Mining::map_file(){
	ofstream in("miner/bd.txt");
	if(!in) {
		cout << "Cannot open file.";
		return ;
	}

	for(int i=0 ; i< sizeES ; i++){
		vector<Edge*> * edges = ES->at(i)->getEdges();
		int n = ES->at(i)->V;
		for(int e = 0; e < edges->size(); e++){
			if(edges->at(e)->usedEdge){
				int smaller, bigger;
				if (edges->at(e)->vertex_a < edges->at(e)->vertex_b){
					smaller = edges->at(e)->vertex_a;
					bigger = edges->at(e)->vertex_b;
				} else{
					smaller = edges->at(e)->vertex_b;
					bigger = edges->at(e)->vertex_a;
				}
				int code = (bigger * n) + smaller;
				in << code << " ";
			}
		}
		in << "\n";
	}
	in.close();
	return;
}


// Lê saída do FPmax e remapeia os padrões em arestas.

void Mining::unmapall_file(int n){
	FILE *fp = fopen("miner/padroes.txt", "r");
	if(!fp) { cout << "ERROR: Could not open padroes.txt file." << endl;  exit(1);}

	int r, supp, tam;
	// // Ler o tamanho e o suporte do padrão corrente
	r = fscanf(fp, "%d;%d;", &tam, &supp);
	// cout << "tam e suporte " << tam  << " " << supp << endl;
	Pattern *p = new Pattern();
	p->support = supp;
	p->size = tam;
	if(!p->support && !p->size){
		numberMine--;
		eschanged = true;
		mined = false;
		//cout << "Sem Padrões Minerados!" << min_sup << endl;
		return;
		min_sup--;
		cout << "Re-MINE: " << min_sup << endl;
		mine();


	}else{
		min_sup = min_sup_orig;
	}
	while(r == 2){
		int elem;

		for(int i = 0; i < tam; i++){

			int l = fscanf(fp, "%d", &elem);
			// cout << "elem: " << elem << endl;
			// cin.get();
			if (l < 1){ cout << "ERROR: fscanf. nothing read." << endl; exit(1);}
			int vertex_a = elem/n;
			int vertex_b = elem%n;
			// cout << "(" << vertex_a << ", " << vertex_b << ") " << endl;
			vector<int> * newPair = new vector<int>(2);
			newPair->at(0) = vertex_a;
			newPair->at(1) = vertex_b;
			p->elements->push_back(newPair);

		}
		// cout << endl;
		listOfPatterns->at(nPatterns++) = p;
		if(p->size < this->smallestPattern)
			this->smallestPattern = p->size;

		if(p->size > this->biggestPattern){
			this->biggestPattern = p->size;
		}

		r = fscanf(fp, "%d;%d;", &tam, &supp);
		p = new Pattern();
		p->support = supp;
		p->size = tam;
	}
	// fclose(fp);
}

bool Mining::updateES(Grafo * s){
	// cout << "SOLUTION COST: " << s->getSolutionCost() << " WORST COST: " << worstCostES << endl;
	// cout << "ES Size: " << sizeES << " Max Size: " << maxSizeES << endl;
	// cin.get();
	if (s->getSolutionCost() < worstCostES || sizeES < maxSizeES) {
		// for(int i = 0 ; i < sizeES ; i++){
		// 	if(ES->at(i)->getSolutionCost() == s->getSolutionCost() ){
		// 		if(IsEqualVectorInt(ES[i]->vetPosCand, ES[i]->numPts, s->vetPosCand, s->numPts)){
		// 			// Se solucao já existir no CE, é descartada.
		// 			if(iterWNC > 0 || eschanged)
		// 				oneMoreIWC();
		// 			cout << "Solution already in ES" << endl;
		// 			return false;
		// 		}
		// 	}
		// }
		eschanged = true;
		mined = false;
		iterWNC = 0;
		if(sizeES < maxSizeES){
			// cout << "blow" << endl;
			ES->at(sizeES++) = s;
		}else{
			// cout << "blei" << endl;
			worstCostES = ES->at(0)->getSolutionCost();
			// cout << "worstCost " << worstCostES << endl;
			worstCostPos = 0;
			for(int i = 1 ; i < sizeES ; i++){
				// cout << "i: " << i << endl;
				if(ES->at(i)->getSolutionCost() > worstCostES ){
					worstCostES = ES->at(i)->getSolutionCost();
					worstCostPos = i;
				}
			}
			// cout << "1" << endl;
			Grafo * oldSol = ES->at(worstCostPos);
			// delete ES->at(worstCostPos);
			// cout << "2" << endl;
			ES->at(worstCostPos) = s;
			delete oldSol;
		}
		// ES worst cost must be updated

		worstCostES = ES->at(0)->getSolutionCost();

		worstCostPos = 0;
		for(int i = 1 ; i < sizeES ; i++){
			if(ES->at(i)->getSolutionCost() > worstCostES ){
				worstCostES = ES->at(i)->getSolutionCost();
				worstCostPos = i;
			}
		}
		// cout << "saiu" << endl;
		return true;
	}
	if(iterWNC > 0 || eschanged)
		oneMoreIWC();
	// cout << "saiu" << endl;
	return false;
}

void Mining::printES(){
	for(int i=0 ; i< sizeES ; i++){
		cout << "Solução " << i << ":" << endl << "\t";
		ES->at(i)->printGraph();
		cout << "Custo " << ES->at(i)->getSolutionCost() << endl;
	}
}

void Mining::printParsedPatterns(RFWLocalRandom * random){
	Pattern * pattern = getRandomPattern(random);
	cout << pattern->size << endl;
	for(int j = 0; j < pattern->size; j++){
		int vertex_a = pattern->elements->at(j)->at(0) + 1;
		int vertex_b = pattern->elements->at(j)->at(1) + 1;
		cout << "x";
		string code = to_string(vertex_a) + "_" + to_string(vertex_b);
		if (vertex_a > vertex_b)
			code = to_string(vertex_b) + "_" + to_string(vertex_a);
		int total_chars = 1 + code.size();
		for (int size = 0; size < (16 - total_chars); size++){
			cout << "_";
		}
		cout << code << endl;
	}
}

void Mining::printPatterns(){
	// for(int i=0 ; i< nPatterns ; i++){
	// 	//cout << "Padrão " << i << ":" << endl << "\t";
	// 	for(int j = 0 ; j < numPts; j++){
	// 		if(listOfPatterns[i]->elements[j] == -1)
	// 			cout << "- ";
	// 		else
	// 			cout << numPosCand*j+listOfPatterns[i]->elements[j]<< " ";
	// 	}
	// 	cout << endl;
	// }
}

//---------------------------------------------------------------------------
