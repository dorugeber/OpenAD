!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(x,y)
        use OAD_intrinsics
        double precision x
        double precision y
        y=x
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision x
        double precision y, py
C$openad INDEPENDENT(x)
        call foo(x,y)
        call foo(2.0,py)
C$openad DEPENDENT(y)
      end subroutine

