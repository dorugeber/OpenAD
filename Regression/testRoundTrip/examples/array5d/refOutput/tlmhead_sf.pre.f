!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(a)
      use OAD_intrinsics
      double precision, dimension(1,1,1,1,1) :: a
      a(1,1,1,1,1) = a(1,1,1,1,1)*2
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
      use OAD_intrinsics
      double precision, dimension(1) :: x
      double precision, dimension(1) :: y
      double precision, dimension(1,1,1,1,1) :: z
C$openad INDEPENDENT(x)
      z(1,1,1,1,1)=x(1)
      call foo(z)
      y(1)=z(1,1,1,1,1)
C$openad DEPENDENT(y)
      end subroutine

