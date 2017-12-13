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
        double precision, dimension(4), intent(in) :: x
        double precision, dimension(4), intent(out) :: y
          integer i,k
C$openad xxx simple loop

        do i=1,2*k
          y(i)=x(i)
        end do
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(4), intent(in) :: x
        double precision, dimension(4), intent(out) :: y
          integer i,j
        integer :: oad_ctmp0
C$openad INDEPENDENT(x)
        oad_ctmp0 = 2
        call bar(x,y,oad_ctmp0)
C$openad DEPENDENT(y)
      end subroutine
