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
      subroutine foo(m1,m2)
        use OAD_intrinsics
        implicit none
        double precision,intent(in) :: m1
        double precision,intent(inout) :: m2
        double precision :: oad_ctmp0
        call oad_s_min(m1,m2,oad_ctmp0)
        m2 = oad_ctmp0
      end subroutine foo

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        implicit none
        double precision, dimension(1), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
        double precision t,t1,p
        double precision :: oad_ctmp0
        double precision :: oad_ctmp1
C$openad INDEPENDENT(x)
        t=2.0D0
        p = sqrt(3.0D0)
        call oad_s_min(t,x(1),oad_ctmp0)
        t1 = oad_ctmp0
C different signature here should trigger activation of both elements
        call oad_s_min(x(1),t1,oad_ctmp1)
        y(1) = oad_ctmp1
        call foo(p,t1)
C$openad DEPENDENT(y)
      end subroutine
