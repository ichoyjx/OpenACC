// We should use it like this way
#pragma acc exit data if(flag) copyout(c[0:n]) delete(a[0:n*n], b[0:n])

//...
// if flag = 0, c should not be copyout!
#pragma acc kernels copyin(c[0:n])


// but it there will be a collision for c in the present table, weird
// so I have to use it like the following way...


#pragma acc exit data if(flag) copyout(c[0:n]) delete(a[0:n*n], b[0:n])
// if(0) will disable device memory allocate or deallocate
// and data will not be moved
// cross test would be if(1), then device_sum = host_sum

// Using tmp here is to avoid the present table collision
for (int i=0; i<n; i++) {
  tmp[i] = c[i];
 }

// second kernel, c[i] should not be available now
// when flag = 0, c will be copied from host, which is 0.0
device_sum = 0.0;
#pragma acc kernels copyin(tmp[0:n])
