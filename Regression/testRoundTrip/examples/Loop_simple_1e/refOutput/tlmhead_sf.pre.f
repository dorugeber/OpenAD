!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine foo(x,y)
      use OAD_intrinsics
      double precision :: x,y
        y=x
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(2), intent(in) :: x
        double precision, dimension(2), intent(out) :: y
          integer i,j
C$openad INDEPENDENT(x)
C$openad xxx simple loop
       j=1
       do i=1,2
          call foo(x(j),y(j))
       end do
       j=2
       call foo(x(j),y(j))
C$openad DEPENDENT(y)
      end subroutine
