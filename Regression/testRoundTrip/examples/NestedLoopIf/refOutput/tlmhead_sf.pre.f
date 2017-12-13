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
        double precision, dimension(2), intent(inout) :: x
        double precision, dimension(2), intent(out) :: y
          integer i
C$openad INDEPENDENT(x)
          do i=1,2
            if (i==1) then
              y(i) = x(i)*x(i)
            else
              y(i) = x(i)+x(i)
            end if
            x(i)=0.0
          end do
C$openad DEPENDENT(y)
      end subroutine
