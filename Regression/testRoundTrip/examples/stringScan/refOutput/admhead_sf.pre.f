!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(x,y,a)
        use OAD_intrinsics
        double precision, intent(inout) :: x
        double precision, intent(inout) :: y
        character (len=3) :: a
C$openad INDEPENDENT(x)
        if (scan(a,'a')==3) then
           y=x
          else
             y = 2*x
          end if
C$openad DEPENDENT(y)
      end subroutine



C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(1), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
C$openad INDEPENDENT(x)
        call foo(x(1),y(1),'bla')
C$openad DEPENDENT(y)
      end subroutine
