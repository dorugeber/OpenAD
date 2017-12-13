!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad xxx Template ad_template.f
      subroutine head(x,y)
      use OAD_intrinsics
      double precision, dimension(1) :: x,y
C$openad INDEPENDENT(x)
         goto 1
 3    x(1) = x(1)*2
         goto 4
 2    x(1) = x(1)*3
         goto 3
 1    x(1) = x(1)*x(1)
         goto 2
 4    y(1)=x(1)
C$openad DEPENDENT(y)
      end subroutine
