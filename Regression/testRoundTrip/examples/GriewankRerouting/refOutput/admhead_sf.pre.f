!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine ad_reroute (x1, x2, y1, y2)
        use OAD_intrinsics
        double precision, intent(in):: x1,x2
        double precision, intent(inout) :: y1,y2
        double precision t1,t2,t3,t4
C$openad INDEPENDENT(x1)
C$openad INDEPENDENT(x2)
        t1=x1
          t2=x2
          t3 = t1/t2
        t4 = t1*t2
        y1 = t3*t4
        y2 = t3/t4
C$openad DEPENDENT(y1)
C$openad DEPENDENT(y2)
      end subroutine
