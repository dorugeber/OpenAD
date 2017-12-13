!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module m
      use OAD_intrinsics
      public :: foo
         interface foo
           module procedure foo_i
         end interface
      contains
          subroutine foo_i(x)
           double precision :: x
           x = x*2
         end subroutine
      end module

C$openad XXX Template ad_template.f
      subroutine head(x,y)
      use OAD_intrinsics
      use m
      double precision, dimension(1), intent(inout) :: x
      double precision, dimension(1), intent(inout) :: y
C$openad INDEPENDENT(x)
          call foo(x(1))
          y(1)=x(1)
C$openad DEPENDENT(y)
      end subroutine
