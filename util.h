/*
 * util.h
 * Oil Collecting Vehicle Routing Problem
 * Some utilities Definition . ..
 */

 // For compiling with Mersenne twister random number generator include MTWISTER
 // compiling directive

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <sys/times.h>
#include <sstream> // usar ostringstream

using namespace std;

// Auxiliares
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

void randomize();

void setSeed(const unsigned int seed);

/* generates an integer i in {0,...,maxValue-1} */
unsigned int intRandom(const unsigned int maxValue);

unsigned long genrand_int32(void);
unsigned long genrand_int32_limit(unsigned long);

double doubleRandom(const double maxValue);

double wallClock();

double cpuTime();

void PrintVectorInt(vector<int> &load);
void PrintVectorInt(int* v,int n);

void PrintVectorDouble(vector<double> &load);

bool IsEqualVectorInt(vector<int> &a, vector<int> &b);
bool IsEqualVectorInt(int* a, int sizea, int*  &b, int sizeb);

bool PertenceVectorInt(int elem, vector<int> &vec);

#endif /* ifndef UTIL_H */
