!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine head(x1,x2,y1,y2)
        use OAD_intrinsics
        double precision v3,v4,x1,x2,y1,y2
C$openad INDEPENDENT(x1)
C$openad INDEPENDENT(x2)
        v3 = x1*x2
        v4 = x1*v3
          y1 = v3*x2*v4
          y2=sin(v4)
C$openad DEPENDENT(y1)
C$openad DEPENDENT(y2)
      end subroutine
