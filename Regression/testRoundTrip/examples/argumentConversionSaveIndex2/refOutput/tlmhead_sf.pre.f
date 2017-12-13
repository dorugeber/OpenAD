!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module globals
        integer i
      end module globals


      subroutine foo(p)
        use OAD_intrinsics
        use globals
        double precision, intent(inout) :: p
        p = p*2
        i = i+1
      end subroutine foo


C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        use globals
        double precision, dimension(1), intent(in) :: x
        double precision, dimension(1), intent(out) :: y
        double precision, dimension(1) :: a
        integer :: j
C$openad INDEPENDENT(x)
        i = 1
        j = 0
        a(1) = 0.00908583
        call foo(a(i+j))
        call foo(x(1))
        y(1) = x(1)
C$openad DEPENDENT(y)
      end subroutine head

