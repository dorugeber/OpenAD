!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
        interface oad_s_maxval
          module procedure oad_s_maxval_d_1
        end interface
        contains
          subroutine oad_s_maxval_d_1(a,r)
            double precision,intent(in),dimension(:) :: a
            double precision,intent(out) :: r
            integer :: i(1)
              i = maxloc(a)
              r = a(i(1))
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
        double precision :: oad_ctmp0
C$openad INDEPENDENT(x)
        call oad_s_maxval(x,oad_ctmp0)
        y(1) = oad_ctmp0
C$openad DEPENDENT(y)
      end subroutine
