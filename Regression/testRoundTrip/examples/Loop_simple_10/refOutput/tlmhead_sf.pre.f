!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
        subroutine foo(x,y,a,j)
          use OAD_intrinsics
          double precision, dimension(1), intent(in) :: x
          double precision, dimension(1), intent(out) :: y
        integer a(2,2)
          integer i,j
          y(1)=x(1)
C$openad xxx simple loop
          do i=1,2
            if (a(i,j).ne.0) then
              y(1) = y(1)*x(1)*a(i,j)
            else
              y(1)=0.0
            end if
          end do
C$openad DEPENDENT(y)
        end subroutine


C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(1), intent(in) :: x
        double precision, dimension(1), intent(out) :: y
          integer a(2,2)
C$openad INDEPENDENT(x)
C set a
        do i=1,2
            do j=1,2
            a(i,j) = i+j
          end do
        end do
        call foo(x,y,a,2)
C unset a to make sure it is restored
          do i=1,2
            do j=1,2
              a(i,j)=0
            end do
          end do
C$openad DEPENDENT(y)
      end subroutine
