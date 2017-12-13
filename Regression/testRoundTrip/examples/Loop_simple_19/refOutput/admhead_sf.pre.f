!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine foo(a)
        use OAD_intrinsics
        double precision :: a
         a = 2*a
      end subroutine

      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(2), intent(in) :: x
        double precision, dimension(2), intent(out) :: y
          integer :: i,k

C$openad INDEPENDENT(x)
C
C$openad xxx simple loop
          do i=1,2
          k=i
            call foo(x(k))
          k = -19
          end do
          y=x
C$openad DEPENDENT(y)
      end subroutine
