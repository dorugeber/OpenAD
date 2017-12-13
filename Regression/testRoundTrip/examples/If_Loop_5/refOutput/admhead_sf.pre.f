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
        double precision x(2),y(2)
          integer i
C$openad INDEPENDENT(x)
          i=1
          do while (i<3)
            if (i<2) then
              y(2)=sin(x(1))
            else
              y(1)=cos(x(2))
            end if
            i = i+1
          end do
          y(2) = y(1)*y(2)
C$openad DEPENDENT(y)
      end subroutine
