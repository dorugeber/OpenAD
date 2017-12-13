!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C deal with nested scoping and visibility of fVar in the analysis

      subroutine head(x,y)
        use OAD_intrinsics
C$openad XXX Template ad_template.f
        double precision, dimension(1) :: x
        double precision, dimension(1) :: y
C$openad INDEPENDENT(x)
          call foo(x,y)
C$openad DEPENDENT(y)
      end subroutine

      subroutine foo(fx,fy)
        use OAD_intrinsics
C$openad XXX Template ad_template.f
        double precision, dimension(1) :: fx
        double precision, dimension(1) :: fy
        double precision :: fVar
          call bar(fx,fy)
        contains
          subroutine bar(bx,by)
            double precision, dimension(1) :: bx
            double precision, dimension(1) :: by
            fVar=bx(1)
            by(1)=fVar
            end subroutine
      end subroutine
