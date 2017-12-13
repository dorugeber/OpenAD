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
        double precision,intent(in),dimension(2),target :: x
        double precision,intent(out),dimension(2) :: y

        double precision,dimension(:),pointer :: p

C$openad INDEPENDENT(x)
        p => x
        y = x+p
C$openad DEPENDENT(y)
      end subroutine

