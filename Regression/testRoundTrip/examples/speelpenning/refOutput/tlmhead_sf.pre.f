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
        double precision, dimension(10), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
          integer i
C$openad INDEPENDENT(x)
          do i=1,10
            if (i==1) then
              y(1)=x(1)
            else
              y(1) = y(1)*x(i)
            end if
          end do
C$openad DEPENDENT(y)
      end subroutine
