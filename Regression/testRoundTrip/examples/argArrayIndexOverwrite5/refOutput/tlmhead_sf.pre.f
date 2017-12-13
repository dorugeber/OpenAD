!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(a,k)
        use OAD_intrinsics
        double precision a
        integer k
        a = a*k
        k = 2*k
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(2) :: x
        double precision y
        integer i,j
C$openad INDEPENDENT(x)
        i=1
        j=1
        call foo(x(i+j),i)
        call foo(x(i-j),i)
        y = x(1)*x(2)
C$openad DEPENDENT(y)
      end subroutine

