#ifndef MiningH
#define MiningH

#include <sstream>
#include <cstdlib>
#include "util.h"
#include "Forest_Steiner.hpp"

class Pattern{
			public:
				// int* elements, *usage;	//
				vector<vector<int>*> * elements;
				int support, size;
				Pattern(){ elements = new vector<vector<int>*>(); support = 0; size = 0;};
				~Pattern(){delete[] elements;};
				// // Assignment operator overload
				// Pattern &operator=(const Pattern &other) {
				// 	support = other.support;
				// 	size = other.size;
				// 	for(int i=0;i<numPts;i++){
				// 		elements[i] = other.elements[i];
				// 		usage[i] = other.usage[i];
				// 	}
				// 	return *this;
				// }
		};

class Mining{

	private:
		int nPatterns,sizeES,maxSizeES, min_sup, min_sup_orig, worstCostPos, currentPattern, gamma;
		double worstCostES;
		vector<Pattern*> * listOfPatterns;		// Lista de padroes minerados pelo FPmax
		vector<Grafo*> * ES;    		// lista das melhores solucoes
		bool eschanged,mined;
		int iterWNC, numberMine;
		RFWLocalRandom * random;




	public:


		Mining();
		Mining(int _tamCE, int _suporte, int _gamma, RFWLocalRandom * random);
		~Mining();
		void mine(int = 10);
		void map_file();
		void unmapall_file(int n);
		void printPatterns();
		void printParsedPatterns();
		///// Manipula Conjunto Elite
		void printES();
		bool updateES(Grafo * s);
		int getSizeCE(){ return sizeES;};
		bool isStableES() { return itersWithoutChange() >= gamma;};
		double getWorstCostES() { return worstCostES;};
		int itersWithoutChange() {return iterWNC;};
		void oneMoreIWC() { iterWNC++;};
		bool EShasChanged(){ return eschanged;};
		void resetESConfig() { eschanged = false;};
		bool hasMined() {return mined;};
		int getNumberOfMine() {return numberMine;};
		int getNumberOfPatterns() {return nPatterns;};
		int getSupport() {return min_sup_orig;};

		///// Manipula padrões
		void nextPattern(){(currentPattern < nPatterns -1 ) ? (currentPattern++) : (currentPattern = 0); }
		Pattern* getCurrentPattern() { return listOfPatterns->at(currentPattern);};

};


//---------------------------------------------------------------------------
#endif
