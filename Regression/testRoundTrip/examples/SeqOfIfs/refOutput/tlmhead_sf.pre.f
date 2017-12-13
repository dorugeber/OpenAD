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
        double precision, dimension(2), intent(in) :: x
        double precision, dimension(2), intent(out) :: y
C$openad INDEPENDENT(x)
          if (x(1)<=x(2)) then
            y(1) = x(2)-x(1)
          else
            y(1) = x(1)-x(2)
          end if
          if (y(1)==0) then
            y(2)=x(1)
          else
            y(2)=y(1)
          end if
C$openad DEPENDENT(y)
      end subroutine
