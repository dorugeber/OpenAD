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
        double precision, dimension(4), intent(inout) :: x
        double precision, dimension(4), intent(inout) :: y
        double precision t1,t2,t3
C$openad INDEPENDENT(x)
        t1 = x(1)/x(2)
        t2 = x(3)*x(4)
          t3 = t1*t2
        y(1) = t1*t3
        y(2) = 1./t3
        y(3)=sin(t3)
        y(4) = t3*t2
C$openad DEPENDENT(y)
      end subroutine
