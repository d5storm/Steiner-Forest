/*
 * util.c
 * Oil Collecting Vehicle Routing Problem
 * Some utilities Implementation ...
 */

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include <sys/timeb.h>
#include <sys/resource.h>

/* Code from Mersenne twister random number generator */
/* http://www.math.keio.ac.jp/~matumoto/emt.html      */

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s) {
	mt[0]= s & 0xffffffffUL;
	for (mti=1; mti<N; mti++) {
		mt[mti] =
			(1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
		/* In the previous versions, MSBs of the seed affect   */
		/* only MSBs of the array mt[].                        */
		/* 2002/01/09 modified by Makoto Matsumoto             */
		mt[mti] &= 0xffffffffUL;
		/* for >32 bit machines */
	}
}
unsigned long genrand_int32_limit(unsigned long limit) {
	return genrand_int32()%limit;
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void) {
	unsigned long y;
	static unsigned long mag01[2]={0x0UL, MATRIX_A};
	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (mti >= N) { /* generate N words at one time */
		int kk;

		if (mti == N+1)   /* if init_genrand() has not been called, */
			init_genrand(5489UL); /* a default initial seed is used */

		for (kk=0;kk<N-M;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (;kk<N-1;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mti = 0;
	}

	y = mt[mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void) {
	return genrand_int32()*(1.0/4294967295.0);
	/* divided by 2^32-1 */
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void) {
	return genrand_int32()*(1.0/4294967296.0);
	/* divided by 2^32 */
}

void randomize() {
    unsigned int seed;
	seed = (unsigned int)time((time_t *)NULL);
	printf("Using mersenne twister random number generator.\n");
	init_genrand(seed);
	printf("Random seed is %d\n", seed);
}

void setSeed(const unsigned int seed) {
	init_genrand(seed);
}

inline unsigned int intRandom(const unsigned int maxValue) {
	return ((double)genrand_real2())*((double)maxValue);
}

inline double doubleRandom(const double maxValue) {
	return maxValue * genrand_real1();
}

double wallClock() {
	struct timeb tp;
	double mili;

	ftime(&tp);
	mili = (double)( (tp.time)+((double)tp.millitm)/1000);

	return mili;
}

double cpuTime() {
	static struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	return ((double)usage.ru_utime.tv_sec)+(((double)usage.ru_utime.tv_usec)/((double)1000000));
}

//Edited: Marcos Guerine
void PrintVectorInt(int* vec, int n){
	for (int i=0;i< n;i++){
		cout << vec[i] << " ";
	}
}


void PrintVectorInt(vector<int> &vec){
	for (unsigned int i=0;i< vec.size();i++){
		cout << vec[i] << " ";
	}
}

void PrintVectorDouble(vector<double> &vec){
	for (unsigned int i=0;i< vec.size();i++){
		cout << vec[i] << " ";
	}
}

bool IsEqualVectorInt(vector<int> &a, vector<int> &b){
	if(a.size() != b.size()){ 
		return false;
	}else{
		for(unsigned int i=0;i < a.size(); i++){
			if(a[i] != b[i]){
				return false;
			}
		}
	}
	return true;
}

bool IsEqualVectorInt(int* a, int sizea, int*  &b, int sizeb){
	if(sizea != sizeb){ 
		return false;
	}else{
		for(int i=0;i < sizea; i++){
			if(a[i] != b[i]){
				return false;
			}
		}
	}
	return true;
}


bool PertenceVectorInt(int elem, vector<int> &vec){
	for(unsigned int i =0; i< vec.size();i++){
		if(vec[i] == elem){
			return true;
		}
	}
	return false;
}

