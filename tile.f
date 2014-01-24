! OpenACC 2.0 introduces a new feature tile
! following example has a 64-long tile in
! the "j" loop, and a 4-wide tile in the "i" loop

!$acc parallel
!$acc loop tile(64,4) gang vector
      do i = 1, n
         do j = 1, m
            a(j,i) = (b(j-1,i)+b(j+1,i)+ &
            b(j,i-1)+b(j,i+1))*0.25
         enddo
      enddo
!$acc end parallel
