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
        double precision a
        a = a*2
      end subroutine

C$openad XXX Template ad_template.f
      subroutine bar(a,k)
        use OAD_intrinsics
        double precision a
        integer k
        k = k*2
        a = a*k
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(2) :: x
        double precision y
        integer i,j
C$openad INDEPENDENT(x)
        i=1
        do j=1,2
          call foo(x(j))
          call bar(x(j),i)
        enddo
        y = x(1)*x(2)
C$openad DEPENDENT(y)
      end subroutine

