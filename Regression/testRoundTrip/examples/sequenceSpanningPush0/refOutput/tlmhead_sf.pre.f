!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine foo(x,t)
        use OAD_intrinsics
        double precision, intent(in) :: x
        double precision, intent(inout) :: t
        t = t*x
      end subroutine foo


C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(1), intent(in) :: x
        double precision, dimension(1), intent(out) :: y
        double precision :: t
C$openad INDEPENDENT(x)
        i = 1
        t = x(i)
        call foo(x(i),t)
        y(i) = t
C$openad DEPENDENT(y)
      end subroutine head

