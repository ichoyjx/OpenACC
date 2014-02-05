/*
 * First, introduce a small shift operation code
 * what is the result?
 * http://codepad.org/pjtQUGKk
 */
#include <stdio.h>

int main() {
    printf("%d\n", 1>>32);
    return 0;
}

/*
 * How does it work when running on GPU?
 * here is the code snippet of the atomic test case
 */
int i;
int x = 1;
int tmp[n];
#pragma acc parallel loop num_gangs(n)
  for (i=0; i<n; i++) {
#pragma acc atomic update
    x = 1 << x;

    tmp[i] = x;
  }
/*
 * Is that supposed to be: 1, 2, 4, 16, 65536, ... what is the next?
 * since OpenACC is implemented using CUDA, there should be a barrier
 * after atomic. So guess what the value of tmp[i] is?
 */
/*
 * It turns out that all the tmp[i] = 4 for two commercial compilers.
 * The bug of compiler?
 * No. Check http://choorucode.com/2004/12/20/c-shift-operator-mayhem/
 * So it is hw specific and I cannot blame anything.
 *
 * That is why I set n = 4, then x would be up to 16, then everybody
 * is happy now.
 */
