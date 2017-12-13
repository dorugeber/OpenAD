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
      double precision x(1),y(1)
      integer i
C$openad INDEPENDENT(x)
      y(1)=x(1)
      i = 1+INT(x(1)-1)
C$openad DEPENDENT(y)
      end subroutine head
