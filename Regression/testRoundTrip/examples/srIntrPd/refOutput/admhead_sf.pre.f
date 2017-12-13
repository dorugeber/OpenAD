!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
        interface oad_s_max
          module procedure oad_s_max_d
        end interface
        interface oad_s_min
          module procedure oad_s_min_d
        end interface
        contains
          subroutine oad_s_max_d(a0,a1,r)
            double precision,intent(in) :: a0
            double precision,intent(in) :: a1
            double precision,intent(out) :: r
            if (a0>a1) then
              r = a0
            else
              r = a1
            end if
          end subroutine
          subroutine oad_s_min_d(a0,a1,r)
            double precision,intent(in) :: a0
            double precision,intent(in) :: a1
            double precision,intent(out) :: r
            if (a0<a1) then
              r = a0
            else
              r = a1
            end if
          end subroutine
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine head(x,y)
      use OAD_intrinsics
      double precision, dimension(2), intent(inout) :: x
      double precision, dimension(1), intent(inout) :: y
      double precision :: p1, p2
        double precision :: oad_ctmp0
        double precision :: oad_ctmp1
C$openad INDEPENDENT(x)
          p1=2.4D0
          p2 = -30.0D0
          call oad_s_min(x(1),x(2),oad_ctmp0)
          call oad_s_max(p1,p2,oad_ctmp1)
          y(1) = oad_ctmp0*oad_ctmp1
C$openad DEPENDENT(y)
      end subroutine
