/* */

for (int idx=0; idx<2; idx++) {

#pragma acc kernels async(acc_async_sync)
#pragma acc loop gang
  for(int i=0; i<n; i++) {
    c[i] += 1.0;
  }

  is_sync = acc_async_test_all(); // should be non-zero
  if (idx == 1) { kernels_err += (is_sync == 0); }

#pragma acc kernels async(acc_async_noval)
#pragma acc loop gang
  for(int i=0; i<n; i++) {
    c[i] -= 1.0;
  }

  is_sync = acc_async_test_all(); // should be 0
  if (idx == 0) { kernels_err += (is_sync != 0); }

 }
