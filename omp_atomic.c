/*
 * Is this an usual way to test atomic?
 *
 *     http://gcc.gnu.org/bugzilla/show_bug.cgi?id=45423
 *
 */

#include <stdio.h>

#define NUM_ACCEL 4
#define ARRAYSIZE 10000
#define OPS_PER_ACCEL ARRAYSIZE/NUM_ACCEL

int add () {
  int result=0;
  int i;
#pragma omp parallel for num_threads(NUM_ACCEL) private(i)
  for (i=0; i<ARRAYSIZE; i) {
#pragma omp atomic
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
