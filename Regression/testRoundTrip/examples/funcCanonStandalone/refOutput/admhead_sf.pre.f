!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
        interface oad_s_max
          module procedure oad_s_max_d
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
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      double precision function bar(x)
        use OAD_intrinsics
        double precision,intent(in) :: x
        bar = max(x,4.0D0)
      end function bar
      subroutine oad_s_bar(x,bar)
        use OAD_intrinsics
        double precision,intent(in) :: x
        double precision :: oad_ctmp0
        double precision :: oad_ctmp1
        double precision,intent(out) :: bar
        oad_ctmp1 = 4.0D0
        call oad_s_max(x,oad_ctmp1,oad_ctmp0)
        bar = oad_ctmp0
      end subroutine oad_s_bar

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        implicit none
        double precision, dimension(1), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
        double precision :: t
        double precision :: bar
        double precision :: oad_ctmp0
        double precision :: oad_ctmp1
C$openad INDEPENDENT(x)
        call oad_s_bar(x(1),oad_ctmp0)
        y(1) = oad_ctmp0*2.0D0
        call oad_s_bar(y(1),oad_ctmp1)
        t = oad_ctmp1
C$openad DEPENDENT(y)
      end subroutine head

