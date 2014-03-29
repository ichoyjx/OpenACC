module common
  implicit none
  real :: x, y=5.
end module common

! main driver
program test
  implicit none
  call sub1
  call sub2
  call sub3
end program test

!---
subroutine sub1
  use common, only: x
  implicit none
  real :: y
  x=3. ! x value will be changed
  y=1. ! y won't
  write(*,*) x,y
end subroutine sub1

!---
subroutine sub2
  use common, only: x
  implicit none
  write(*,*) x
  x=7. ! x value will be changed again
end subroutine sub2

!---
subroutine sub3
  use common
  implicit none
  write(*,*) x, y ! x = 7.0, y = 5.
end subroutine sub3
