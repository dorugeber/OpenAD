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
        double precision, dimension(10), intent(in) :: x
        double precision, dimension(10), intent(out) :: y
        double precision pi
          integer i
C$openad INDEPENDENT(x)
          pi=3.1415
          do i=1,9
            if (i>5) then
              y(i) = pi*sin(x(i))
C	      print *, 'x(',i,')=',x(i)
C	      print *, 'y(',i,')=',y(i)
            else
              y(i) = pi+cos(x(i))
            end if
          end do
          y(10) = y(1)*y(9)
C$openad DEPENDENT(y)
      end subroutine
