!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module globals
        double precision :: aGlobalActive
      end module globals


      subroutine foo(p)
        use OAD_intrinsics
        use globals
        double precision, intent(inout) :: p
        aGlobalActive = aGlobalActive*p
      end subroutine foo


C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        use globals
        double precision, dimension(1), intent(in) :: x
        double precision, dimension(1), intent(out) :: y
        double precision :: anInactive
C$openad INDEPENDENT(x)
        anInactive = 44.456
        aGlobalActive = 1.61803399
        call foo(x(1))
        call foo(anInactive)
        y(1) = sin(aGlobalActive)
C$openad DEPENDENT(y)
      end subroutine head

