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
          double precision, dimension(1) :: localx
C$openad INDEPENDENT(x)
        localx(1) = 2*x(1)
          y(1) = localx(1)*localx(1)
C$openad DEPENDENT(y)
      end subroutine
