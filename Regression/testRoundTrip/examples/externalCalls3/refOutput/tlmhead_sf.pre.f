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
        double precision :: x
        double precision :: y
        double precision :: t1, t2
C$openad INDEPENDENT(x)
        t1=x
        call barExt(x,t2)
          y=t2
C$openad DEPENDENT(y)
      end subroutine
