!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module m
      use OAD_intrinsics
        interface foo
           module procedure fooI
        end interface
        contains
      subroutine fooI(x)
C$openad XXX Template ad_template.f
        double precision :: x(:)
      end subroutine
        end module

      subroutine head(x,y)
        use OAD_intrinsics
        use m
C$openad XXX Template ad_template.f
        double precision, dimension(:) :: x,y
C$openad INDEPENDENT(x)
        y=x
          call foo(y)
C$openad DEPENDENT(y)
      end subroutine

