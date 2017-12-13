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
        double precision, dimension(1), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
          logical l
C$openad INDEPENDENT(x)
          l = .true.
          if (l) then
            y(1) = x(1)*4
          else
            y(1) = x(1)*2
          end if
C$openad DEPENDENT(y)
      end subroutine
