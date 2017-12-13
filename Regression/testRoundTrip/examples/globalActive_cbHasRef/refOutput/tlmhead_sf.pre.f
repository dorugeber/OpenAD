!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine bar()
      use OAD_intrinsics
      call foo()
      call foo1()
      end subroutine
      subroutine foo()
      use OAD_intrinsics
      common /cb/ g
      double precision g
      common /cb1/ g1
      double precision g1
      g1 = g*2.0
      end subroutine
      subroutine foo1()
      use OAD_intrinsics
      common /cb/ g
      double precision g
      common /cb1/ g1
      double precision g1
      g = g1*3.0
      end subroutine
      subroutine baz()
      use OAD_intrinsics
      common /cb/ g
      double precision g
      common /cbxy/ x,y
      double precision x(1),y(1)
          g=x(1)
          call bar()
          y(1)=g
       end subroutine
      subroutine head()
      use OAD_intrinsics
      common /cbxy/ x,y
      double precision x(1),y(1)
C$openad INDEPENDENT(x)
          x=x
C provide some reference
          y=y
C provide some reference
          call baz()
C$openad DEPENDENT(y)
       end subroutine
