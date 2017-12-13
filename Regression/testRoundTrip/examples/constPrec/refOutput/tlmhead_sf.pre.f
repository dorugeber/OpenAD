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
        double precision, dimension(1) :: x
        double precision, dimension(1) :: y
           Real*8 PI
           PARAMETER ( PI = 3.14159265358979323844d0 )
           Real*8 deg2rad
           PARAMETER ( deg2rad = 2.d0*PI/360.d0 )
C$openad INDEPENDENT(x)
          y(1)=cos(deg2rad*x(1))
C$openad DEPENDENT(y)
      end subroutine
