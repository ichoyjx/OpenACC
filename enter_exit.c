// first
#pragma acc enter data copyin(a[0:n*n], b[0:n], c[0:n])

#pragma acc kernels
#pragma acc loop gang independent
  for (int i=0; i<n; i++) {
    real_t tmp=0.0;
#pragma acc loop reduction(+:tmp)
    for (int j=0; j<n; j++) {
      tmp += a[i*n+j] * b[j];
    }
    c[i] = tmp;
  }

#pragma acc exit data delete(a[0:n*n], b[0:n]) copyout(c[0:n])

// second
