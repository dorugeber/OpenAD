!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module m1
      use OAD_intrinsics
        contains
      subroutine foo(x,n)
      double precision:: x(n)
      integer :: n
      end subroutine

      end module

      module m2
      use OAD_intrinsics
      use m1
      contains
      subroutine bar(x,n)
        integer :: n
      double precision :: x(n)
      integer :: i
        integer :: oad_ctmp0
        do i=1,n
          x(i) = x(i)*2
        end do
      i=2
      oad_ctmp0 = size(x)-i
      call foo(x(i),oad_ctmp0)
      end subroutine
      end module

      subroutine head(x,y)
          use OAD_intrinsics
          use m2
        double precision, dimension(4) :: x, y
        integer :: oad_ctmp0
C$openad INDEPENDENT(x)
        oad_ctmp0 = 4
        call bar(x,oad_ctmp0)
        y=x
C$openad DEPENDENT(y)
      end subroutine

