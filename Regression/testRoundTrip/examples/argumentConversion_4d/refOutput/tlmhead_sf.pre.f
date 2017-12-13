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
        double precision, dimension(4):: x
        double precision y
        y = x(2)*x(4)
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(2) :: x
        double precision, dimension (2,2) :: t, p
        double precision :: y, py
C$openad INDEPENDENT(x)
        t(2,1)=x(1)
        t(2,2)=x(2)
        call foo(t,y)
        call foo(p,py)
C$openad DEPENDENT(y)
      end subroutine

