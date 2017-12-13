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
        double precision, dimension(1), intent(in) :: x
        double precision, dimension(1), intent(out) :: y
          integer i,j,k
C$openad INDEPENDENT(x)
          do i=1,1
            y(1)=x(1)
            if (y(1)>0.) then
              y(1)=x(1)
              do j=1,3
                y(1)=x(1)
                do k=1,3
                  y(1)=x(1)
                  if (x(1)>0) then
                    y(1) = y(1)*x(1)
                  end if
                end do
              end do
            else
              y(1) = y(1)/x(1)
            end if
          end do
C$openad DEPENDENT(y)

      end subroutine
