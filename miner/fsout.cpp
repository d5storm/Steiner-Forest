#include "fsout.h"
#include "common.h"

FPSout::FPSout(char *filename)
{
  out = fopen(filename,"wt");
}

FPSout::~FPSout()
{
  if(out) fclose(out);
}

int FPSout::isOpen()
{
  if(out) return 1;
  else return 0;
}

void FPSout::printSet(int length, int *iset, int support)
{
//#ifdef shown 
  fprintf(out, "%d;%d;",length,support);
  for(int i=0; i<length; i++) 
  {
	fprintf(out, "%d ", order_item[iset[i]]);
//	printf("%d ", order_item[iset[i]]);
  }
  //fprintf(out, "(%d)\n", support);
  fprintf(out,"\n");
//  printf("(%d)\n", support);
//#endif
}

void FPSout::printset(int length, int *iset)
{
//#ifdef shown 
  for(int i=0; i<length; i++) 
    fprintf(out, "%d ", order_item[iset[i]]);
//#endif
}

void FPSout::close()
{
	fclose(out);
}

