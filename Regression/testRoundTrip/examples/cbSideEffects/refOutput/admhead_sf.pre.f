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
      g = g*2.0
      end subroutine
      subroutine foo1()
      use OAD_intrinsics
      common /cb/ g
      double precision g
      g = g*3.0
      end subroutine
      subroutine head(x,y)
      use OAD_intrinsics
      common /cb/ g
      double precision g
      double precision, dimension(1), intent(inout) :: x
      double precision, dimension(1), intent(inout) :: y
C$openad INDEPENDENT(x)
          g=x(1)
          call bar()
          y(1)=g
C$openad DEPENDENT(y)
       end subroutine
