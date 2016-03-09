
/* Array-of-Structures vs. Structure-of-Arrays */

// includes
#include <stdlib.h>
#include <stdio.h>
#include "hrt.h"

// Adjust the number of elements based on architecture
#ifdef __arm__
  #define NUMVALUES 1048576
#elif defined __x86_64__
  #define NUMVALUES 67108864
#endif

// You'll need to modifiy this struct
typedef struct nums {
  double mydouble;
  float myfloat;
} myNums;

void init_nums(myNums *);
void compute_averages(myNums *);

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

  // Data structure for the program
  myNums *AoS;  // AoS will be an array of structs
                // You'll want to convert to a struct of arrays

  // Allocate and initialize the array of structs
  // This will also need to change
  AoS  = (myNums *) malloc(NUMVALUES*sizeof(myNums));

  hrt_start();
  init_nums(AoS);  // time the initialization
  hrt_stop();
  fprintf(stdout, "Initialization time of two %d vectors took %s.\n",
      NUMVALUES, hrt_string());
  
  hrt_start();
  compute_averages(AoS);  // time the computation
  hrt_stop();
  fprintf(stdout, "Computation time of two %d vectors took %s.\n",
      NUMVALUES, hrt_string());

  // Free 
  free(AoS);
  return 0;
}

////////////////////////////////////////////////////////////////////////
// You'll need to modify the following two functions to adjust how
// the data structure is accessed. 

void init_nums (myNums *AoS) {
  unsigned int fseed = 2016;
  unsigned int dseed = 2017;

  for (unsigned int i = 0; i<NUMVALUES; ++i) {
    AoS[i].myfloat = (rand_r(&fseed) / (float)RAND_MAX);
    AoS[i].mydouble = (rand_r(&dseed) / (double)RAND_MAX);
  }
}

void compute_averages (myNums *AoS) {
  float sumf = 0.0;
  double sumd = 0.0;

  for (unsigned int i = 0; i<NUMVALUES; ++i) {
    sumf += AoS[i].myfloat; 
  }
  fprintf(stdout, "Average of %d floats = %.5f.\n", 
      NUMVALUES, sumf/(float)NUMVALUES);

  for (unsigned int i = 0; i<NUMVALUES; ++i) {
    sumd += AoS[i].mydouble; 
  }
  fprintf(stdout, "Average of %d floats = %.5lf.\n", 
      NUMVALUES, sumf/(double)NUMVALUES);
}
