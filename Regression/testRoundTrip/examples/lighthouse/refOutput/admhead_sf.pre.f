!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(4) :: x
        double precision, dimension(2) :: y
          double precision :: nu, gamma, omega, t, v
C$openad INDEPENDENT(x)

          nu = x(1)
          gamma = x(2)
          omega = x(3)
          t = x(4)

          v = tan(omega*t)
          y(1) = (nu*v)/(gamma-v)
          y(2) = gamma*y(1)

C$openad DEPENDENT(y)
      end subroutine
