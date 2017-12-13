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
        double precision, dimension(3), intent(in) :: x
        double precision, dimension(3), intent(out) :: y
          integer i,j,k
C$openad INDEPENDENT(x)
C$openad xxx simple loop

          do i=1,3
          if (i.eq.2) then
             y(i)=x(i)
          else
             y(i) = 2*x(i)
          end if
          end do
C$openad DEPENDENT(y)
      end subroutine
