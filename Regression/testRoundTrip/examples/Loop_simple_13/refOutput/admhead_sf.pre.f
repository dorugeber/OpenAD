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
        double precision s
C$openad INDEPENDENT(x)
          i=1
          y(1)=0.0
        y(2)=0.0
C$openad xxx simple loop
        do i=1,2
            s=x(i)
          if (s.gt.0.D0) then
             s=0.D0
            end if
            y(i) = x(i)+s
          end do
C$openad DEPENDENT(y)
      end subroutine
