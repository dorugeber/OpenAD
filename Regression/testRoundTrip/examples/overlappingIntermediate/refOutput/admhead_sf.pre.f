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
        double precision, dimension(5), intent(in) :: x
        double precision, dimension(3), intent(out) :: y
        double precision t

C$openad INDEPENDENT(x)

          t = x(1)*x(2)*x(3)
          y(1)=sin(t)
          t = t*x(4)*x(5)
          y(2)=sin(t)
          y(3)=cos(t)

C$openad DEPENDENT(y)
      end subroutine
