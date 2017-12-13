!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(x,y,l,u,s)
        use OAD_intrinsics
        double precision, dimension(5), intent(in) :: x
        double precision, intent(inout) :: y
          integer i,l,u,s
C$openad xxx simple loop
          do i=l,u,s
            if (i.lt.5) then
              y = y*x(i)
            end if
          end do
      end subroutine


C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(5), intent(in) :: x
        double precision, dimension(4), intent(out) :: y
C$openad INDEPENDENT(x)
          y=x(1:4)
        call foo(x,y(1),1,3,1)
        call foo(x,y(2),3,1,-1)
        call foo(x,y(3),1,5,2)
        call foo(x,y(4),5,1,-2)
C$openad DEPENDENT(y)
      end subroutine
