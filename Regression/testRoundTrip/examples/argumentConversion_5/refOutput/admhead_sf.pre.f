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
        double precision x(2,1)
        double precision y
        y = x(1,1)*x(2,1)
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(2) :: x,px
        double precision y,py
C$openad INDEPENDENT(x)
        call foo(x,y)
        call foo(px,py)
C$openad DEPENDENT(y)
      end subroutine

