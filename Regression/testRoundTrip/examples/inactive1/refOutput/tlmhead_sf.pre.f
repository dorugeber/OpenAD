!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module m
      use OAD_intrinsics
       interface i
         module procedure p1
         module procedure p2
       end interface
     
      contains
       subroutine p1 (x)
       double precision :: x
       end subroutine
       subroutine p2 (x)
       integer :: x
       end subroutine
      end module
     
      subroutine foo (x)
       use OAD_intrinsics
       use m
       double precision :: x
       integer :: theInt
       theInt=2
       call i(x)
       x = x*theInt
       call i(x)
       call i(theInt)
       call i(x)
      end subroutine
     
      subroutine head(x,y)
       use OAD_intrinsics
       double precision :: x(1),y(1)
C$openad INDEPENDENT(X)
       x = 2*x
       call foo(x)
       y=x
C$openad DEPENDENT(y)
      end subroutine
