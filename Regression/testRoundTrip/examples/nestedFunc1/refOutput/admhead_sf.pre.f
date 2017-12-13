!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine head(j,x,y)
      use OAD_intrinsics
      double precision, dimension(j), intent(in) :: x
        double precision, dimension(1), intent(out) :: y
        integer j
C$openad INDEPENDENT(x)
        y(1)=1.0
        call foo()
C$openad DEPENDENT(y)
      contains
      subroutine foo()
      integer i
      do i=1,j
         y(1) = y(1)*x(j)
      end do
      end subroutine
      end subroutine
