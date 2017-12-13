!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with scalar operations
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      subroutine head(x, y) 
        double precision, intent(in) :: x
        double precision, intent(inout) :: y
        double precision t1, t2
        integer i

        i=1
        t1=sin(x)*y
        t2=t1*x+i
        y=t1+t2
      end subroutine
      
      program simple1

      double precision :: x, y
      x = 10
      y = 100
      call head(x, y)
      write(*,*) y

      end program
