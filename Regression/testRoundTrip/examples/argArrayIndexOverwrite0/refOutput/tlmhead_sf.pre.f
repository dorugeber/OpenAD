!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(x,y,k)
        use OAD_intrinsics
        double precision x,y
        integer k
        y = x*k
        k = k+2
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(2) :: x
        double precision y
        integer k
C$openad INDEPENDENT(x)
        k=1
        call foo(x(k),y,k)
C$openad DEPENDENT(y)
      end subroutine

