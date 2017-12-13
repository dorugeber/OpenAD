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
        double precision, dimension(:) :: x
        double precision, dimension(:) :: y
C$openad INDEPENDENT(x)
        call barExt(x)
        y(1) = x(1)+x(2)
          y(2) = -x(1)-x(2)
C$openad DEPENDENT(y)
      end subroutine
