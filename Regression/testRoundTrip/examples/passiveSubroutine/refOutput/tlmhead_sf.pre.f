!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine head(x,y)
      use OAD_intrinsics
      double precision x(1),y(1)
      double precision p,q
C$openad INDEPENDENT(x)
      call foo(x(1),y(1))
      p=1.0
      call bar(p,q)
C$openad DEPENDENT(y)
      end subroutine head

C$openad XXX Template ad_template.f
      subroutine foo(a,b)
      use OAD_intrinsics
      double precision a,b
      b = a*a
      end subroutine

C$openad XXX Template ad_template.f
      subroutine bar(a,b)
      use OAD_intrinsics
      double precision a,b
      b=cos(a)
      end subroutine
