!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine bar(c,d)
        use OAD_intrinsics
        double precision c,d
        d = c*c
      end subroutine

C$openad XXX Template ad_template.f
      subroutine foo(a,b)
        use OAD_intrinsics
        double precision a,b
        call bar(a,b)
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(1) :: x
        double precision, dimension(1) :: y
C$openad INDEPENDENT(x)
        call foo(x(1),y(1))
C$openad DEPENDENT(y)
      end subroutine
