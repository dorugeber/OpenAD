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
        double precision, dimension(2), intent(inout) :: y
          double precision, dimension(2) ::t
          integer i
C$openad INDEPENDENT(x)
        do i=1,2
          t(i) = 2*x(i)
            y(i) = t(i)*4
          end do
C$openad DEPENDENT(y)
      end subroutine
