!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod
        double precision x(1)
        double precision y(1)
      end module

!$openad xxx file_start [head.f]
      module m
        double precision :: g
      end module
      module m1
        double precision :: g1
      end module
      subroutine bar()
      use OAD_intrinsics
      use m
      use m1
      call foo()
      call foo1()
      end subroutine
      subroutine foo()
      use OAD_intrinsics
      use m
      use m1
      g1 = g*2.0
      end subroutine
      subroutine foo1()
      use OAD_intrinsics
      use m
      use m1
      g = g1*3.0
      end subroutine
      subroutine baz()
      use OAD_intrinsics
      use m
      use m1
      use all_globals_mod
          g=x(1)
          call bar()
          y(1)=g
       end subroutine
      subroutine head()
      use OAD_intrinsics
      use all_globals_mod
      use m
      use m1
C$openad INDEPENDENT(x)
          call baz()
C$openad DEPENDENT(y)
       end subroutine
