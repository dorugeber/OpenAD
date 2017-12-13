!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(x,l)
        use OAD_intrinsics
        double precision:: x
        logical :: l
        l = (x.ne.0.0D0)
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(1) :: x
        double precision, dimension(1) :: y
          logical :: l
C$openad INDEPENDENT(x)
          call foo(x(1),l)
          if (l) then
            y(1)=x(1)
          end if
C$openad DEPENDENT(y)
      end subroutine
