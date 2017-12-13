!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
       subroutine foo()
          use OAD_intrinsics
          implicit double precision (s,t)
          common /cb/ s,t
          t=sin(s)
       end subroutine

C$openad XXX Template ad_template.f
       subroutine head(x,y)
          use OAD_intrinsics
          implicit double precision (s,t)
          common /cb/ s,t
          double precision, dimension(1), intent(inout) :: x
          double precision, dimension(1), intent(inout) :: y
C$openad INDEPENDENT(x)
          s=x(1)
          call foo()
          y(1)=t
C$openad DEPENDENT(y)
       end subroutine
