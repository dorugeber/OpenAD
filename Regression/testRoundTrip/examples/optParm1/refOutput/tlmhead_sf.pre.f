!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine foo(a,b,c,d)
      use OAD_intrinsics
      double precision :: a,c
      double precision, optional :: b,d
      c = 2*a
      if (present(b).and.present(d)) then
      d = 3*b
      end if
      end subroutine

      subroutine head(x,y)
      use OAD_intrinsics
      interface
       subroutine foo(a,b,c,d)
       double precision :: a,c
       double precision, optional :: b,d
       end subroutine
      end interface
      double precision :: x(3),y(3),u,v
C$openad INDEPENDENT(x)
C$openad DEPENDENT(y)
      call foo(a=x(1),c=y(1))
      call foo(x(2),x(3),y(2),y(3))
      end subroutine
