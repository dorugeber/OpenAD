   program myprog
      implicit none

      integer :: n, i, j
      parameter( n = 200 )

! a derived type define
      type test
        integer :: list(n)
      end type test

      type(test) :: rows(n)

      do i = 1,  n
         do j = 1, n
            rows(i)%list(j) = 123
         end do
      end do
      if(rows(10)%list(15).EQ.123) then
	    print *, "OK"
	  else
		print *, "failed"
	  end if
	  end
 