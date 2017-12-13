!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        implicit none
        double precision, dimension(2) :: x
        double precision, dimension(1) :: y
C$openad INDEPENDENT(x)
          call foo()
          call bar()
C$openad DEPENDENT(y)
        contains
          subroutine foo()
            implicit none
            y(1) = x(1)*x(2)
          end subroutine
          subroutine bar()
            implicit none
          y(1) = y(1)*sin(x(1))*cos(x(2))
          end subroutine
      end subroutine
