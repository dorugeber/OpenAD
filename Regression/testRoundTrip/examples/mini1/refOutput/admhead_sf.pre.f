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
        double precision,intent(in) :: x
        double precision,intent(out) :: y
C$openad INDEPENDENT(x)
        y=sin(x*x)
C$openad DEPENDENT(y)
      end subroutine
