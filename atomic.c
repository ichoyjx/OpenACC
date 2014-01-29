// essentially the OpenMP atomic operations
// atomic update, read, write, capture
// only native data lengths supported ?

#pragma acc parallel loop
for( j = 1; j < n-1; ++j ) {
  y = x[j];
  i = y & 0xf;
#pragma acc atomic update
  ++bin[i];
 }
