!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      SUBROUTINE head (x1, x2, y)
      use OAD_intrinsics
      IMPLICIT none
      double precision,intent(in):: x1, x2
      double precision,intent(out):: y
      double precision:: a, b, c, d, e, f
C$openad INDEPENDENT(x1)
C$openad INDEPENDENT(x2)
      a = (1.0d0/(x1/x2))
      b = a*x2
      c = x2
C---------------------------------------
      c = sin(c)
      d = a
      f = a
C---------------------------------------
      f = 0.5d0*(f-a)
      e = 0.5d0*(d-a)
      y = c*e+f*b
C$openad DEPENDENT(y)
      END SUBROUTINE head
