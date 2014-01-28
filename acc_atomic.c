/*
 * Is this an usual way to test atomic?
 *
 *     http://gcc.gnu.org/bugzilla/show_bug.cgi?id=45423
 *
 */

#include <stdio.h>
#include <openacc.h>

#define NUM_ACCEL 4
#define ARRAYSIZE 10000
#define OPS_PER_ACCEL ARRAYSIZE/NUM_ACCEL

int add () {
  int result=0;
  int i;
#pragma acc parallel loop num_gangs(NUM_ACCEL)
  for (i=0; i<ARRAYSIZE; i) {
#pragma acc atomic
    result += array[i];
  }
  return result;
}


int main () {
  int i, result;

  result = add();

  //check result
  printf ("Result: %d\n", result);
  if (result==55000) {
    printf("RESULT: PASS\n");
  } else {
    printf("RESULT: FAIL\n");
  }

  return 0;
}
