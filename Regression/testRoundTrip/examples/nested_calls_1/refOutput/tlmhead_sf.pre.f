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
      double precision x(4),y(4)
C$openad INDEPENDENT(x)
      call foo(x(1),x(2),y(1))
      y(2)=sin(x(1)*x(2))
      y(3)=sin(x(3))
      y(4)=cos(x(4))
C$openad DEPENDENT(y)
      end subroutine head

C$openad XXX Template ad_template.f
      subroutine foo(a,b,c)
      use OAD_intrinsics
      double precision a,b,c

      call bar(b)
      c = a*a+b

      end subroutine

C$openad XXX Template ad_template.f
      subroutine bar(a)
      use OAD_intrinsics
      double precision a

      a=cos(a*a)

      end subroutine
