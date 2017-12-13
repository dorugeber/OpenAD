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
          do i=1,2
          do j=1,2
C       this was in here originally: 
C            do while(x(1)<>x(2))
              y(i) = x(i)*x(j)
            end do
          end do
C$openad DEPENDENT(y)
      end subroutine
