!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine bar(x,y, k )
        use OAD_intrinsics
        double precision, dimension(3), intent(in) :: x
        double precision, dimension(3), intent(out) :: y
          integer i,k
C$openad xxx simple loop

        do i=1,min(k,3)
          y(i)=x(i)
        end do
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(3), intent(in) :: x
        double precision, dimension(3), intent(out) :: y
          integer i,j,k
C$openad INDEPENDENT(x)
        k=3
        call bar(x,y,k)
C$openad DEPENDENT(y)
      end subroutine
