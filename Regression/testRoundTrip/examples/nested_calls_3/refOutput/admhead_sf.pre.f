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
      double precision c,d
C$openad INDEPENDENT(x)
      y(1)=x(3)
      Y(2)=x(4)
      call foo(x(1),x(2),c,d)
      y(3) = c*d
      y(4) = c+d
C$openad DEPENDENT(y)
      end subroutine head

C$openad XXX Template ad_template.f
      subroutine foo(a,b,c,d)
      use OAD_intrinsics
      double precision a,b,c,d
      c=sin(a*b)
      d=cos(a+b)
      end subroutine
