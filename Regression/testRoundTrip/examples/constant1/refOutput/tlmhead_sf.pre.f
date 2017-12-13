!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod
          double precision, parameter :: PI = 3.14159265358979323844D0
      end module
!$openad xxx file_start [head.f]
      module anotherModule
          use all_globals_mod
        double precision, parameter :: pi2=PI
      end module


C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        use anotherModule
        double precision, dimension(1) :: x
        double precision, dimension(1) :: y
C$openad INDEPENDENT(x)
          y(1)=sin(x(1)*pi2*2)
C$openad DEPENDENT(y)
      end subroutine
