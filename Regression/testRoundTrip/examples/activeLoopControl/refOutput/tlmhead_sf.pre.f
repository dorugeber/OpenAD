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
        double precision, dimension(1), intent(out) :: y
          integer i
C$openad INDEPENDENT(x)
          y(1)=x(1)
          do i=int(x(1)),int(x(2))+2
            y(1) = y(1)*x(2)
          end do
C$openad DEPENDENT(y)
      end subroutine
