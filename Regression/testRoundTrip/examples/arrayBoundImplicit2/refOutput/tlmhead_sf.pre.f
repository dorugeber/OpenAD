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
        double precision, dimension(2) :: x,y
        double precision, dimension(:), allocatable :: p
        allocate (p(3))
C$openad INDEPENDENT(x)
        p(:2)=x
        y=p(:2)
C$openad DEPENDENT(y)
      end subroutine
