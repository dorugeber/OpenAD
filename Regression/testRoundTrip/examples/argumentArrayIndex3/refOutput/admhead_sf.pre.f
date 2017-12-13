!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(x,y)
        use OAD_intrinsics
        double precision x
        double precision y
        y = x*2
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(2) :: x
        double precision y
        double precision, dimension(2) :: p,q
        integer k,l
C$openad INDEPENDENT(x)
        k=1
        call foo(x(k),y)
C activate q, p stays passive
        q(1)=y
        y=q(1)
        p(1)=1.0
        l=1
        call foo(p(k),q(l))
C$openad DEPENDENT(y)
      end subroutine
