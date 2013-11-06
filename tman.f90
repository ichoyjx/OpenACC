program tdot
  ! pgfortran -mp -acc tman.f90 -lacml
  ! pgfortran -mp -acc tman.f90 -lacml
  ! set OMP_NUM_THREADS=2

  use openacc
  use omp_lib

  integer, parameter :: N = 10000
  real*8 x(N), y(N), z
  integer, allocatable :: offs(:)
  real*8, allocatable :: zs(:)
  real*8 ddot

  nthr = omp_get_max_threads()

  if (nthr .gt. 2) nthr = 2
  call omp_set_num_threads(nthr)

  call random_number(x)
  call random_number(y)
  z = ddot(N,x,1,y,1)
  print *,"Host Serial", z

  !$omp PARALLEL private(i)
  i = omp_get_thread_num()
  call acc_set_device_num(i, acc_device_nvidia)
  !$omp end parallel

  nsec = N / nthr
  allocate(offs(nthr), zs(nthr))
  offs = (/ (i*nsec, i=0, nthr-1) /)
  zs = 0.0d0

  !$omp PARALLEL private(i, j, z)
  i = omp_get_thread_num() + 1
  z = 0.0d0
  !$acc kernels loop copyin( x(offs(i)+1 : offs(i)+nsec), y(offs(i)+1 : offs(i)+nsec) )
  do j = offs(i)+1, offs(i)+nsec
     z = z + x(j) * y(j)
  end do

  zs(i) = z
  !$omp end parallel

  z = sum(zs)
  print *,"Multi-Device Parallel", z
end program tdot
