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
      double precision x(2),y(1)
      integer i
      i=1
C$openad INDEPENDENT(x)
      call foo(x(i),x(i+1),y(1))
C$openad DEPENDENT(y)
      end subroutine head

C$openad XXX Template ad_template.f
      subroutine foo(a,b,c)
      use OAD_intrinsics
      double precision a,b,c
      c = a*a+b
      end subroutine

