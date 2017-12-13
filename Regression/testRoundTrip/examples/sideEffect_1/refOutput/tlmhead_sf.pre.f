!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module globals

        double precision aGlobal

      end module


C$openad XXX Template ad_template.f
      subroutine foo(x,y)
        use OAD_intrinsics

        double precision, dimension(1) :: x
        double precision y
        y = x(1)*x(1)
      end subroutine

C$openad XXX Template ad_template.f
      subroutine bar(x)
          use OAD_intrinsics
          use globals
        double precision :: x
        x = x*aGlobal
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
          use OAD_intrinsics
          use globals
        double precision, dimension(1) :: x
        double precision, dimension(1) :: y
C$openad INDEPENDENT(x)
        aGlobal=2.0
        call bar(x(1))
        call foo(x,aGlobal)
          y(1)=aGlobal
C$openad DEPENDENT(y)
      end subroutine
