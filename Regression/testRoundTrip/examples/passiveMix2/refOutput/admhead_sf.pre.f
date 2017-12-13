!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
        interface oad_s_min
          module procedure oad_s_min_d
        end interface
        contains
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
        double precision, dimension(1), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
          double precision t,t1,t2
        double precision :: oad_ctmp0
        double precision :: oad_ctmp1
        double precision :: oad_ctmp2
        double precision :: oad_ctmp3
C$openad INDEPENDENT(x)
        t=2.0D0
          call oad_s_min(t,x(1),oad_ctmp0)
          t1 = oad_ctmp0
C different signature here should trigger activation of both elements
          call oad_s_min(t1,t,oad_ctmp1)
          y(1) = oad_ctmp1
          oad_ctmp3 = 2.5D0
          call oad_s_min(t,oad_ctmp3,oad_ctmp2)
          t2 = oad_ctmp2
C$openad DEPENDENT(y)
      end subroutine
