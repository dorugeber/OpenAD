!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module m
      use OAD_intrinsics

      type t
        double precision, dimension(2) :: v
      end type

      contains

      subroutine foo ( ta )
      type(t) :: ta(:)
      integer :: i, j
      j=1
      do i = 1, ubound(ta,1)
         ta(i)%v(j) = ta(i)%v(j)*2.0D0
         j = j+1
      end do
      end subroutine
      end module
    
      subroutine head(x,y)
        use OAD_intrinsics
      
        use m

        double precision, dimension(2) :: x
        double precision, dimension(2) :: y
        type(t) , dimension(2) :: ta
        integer i
C$openad INDEPENDENT(x)
        do i=1,2
           ta(i)%v(i) = x(i)
        end do
          call foo(ta)
        do i=1,2
           y(i) = ta(i)%v(i)
          end do
C$openad DEPENDENT(y)
      end subroutine
    
