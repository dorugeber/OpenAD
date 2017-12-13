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
        double precision, dimension(2) :: x
        double precision, dimension(2) :: y
        double precision, dimension(size(x)) :: a,b,c
C$openad INDEPENDENT(x)
        a = tan(x)/sin(x)
        b = a*x+cos(8*x)
        b = b*(tan(x)*a)
        c = a**(4/a)-sqrt(x*a+cos(a)*tan(b))
        y = 5*c+a
C$openad DEPENDENT(y)
        end subroutine

