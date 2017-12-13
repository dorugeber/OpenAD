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
          integer i
C$openad INDEPENDENT(x)
          i=2
        if (x(1).lt.1.0) then
            y(1) = i*x(1)
          goto 2
       endif
       y(1) = y(1)*2
         goto 3
 2       y(1) = y(1)*3
 3       return
C$openad DEPENDENT(y)
      end subroutine
