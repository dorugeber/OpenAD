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
          integer i,j
C$openad INDEPENDENT(x)
C$openad xxx simple loop
       do i=1,2
          y(i)=x(i)
       end do
C$openad DEPENDENT(y)
      end subroutine
