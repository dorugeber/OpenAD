!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine foo(k,a)
        use OAD_intrinsics
        integer :: k,i
        double precision :: a(k)
        do i=1,k
           a(i) = 2*a(i)
        end do
      end subroutine

      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(2), intent(in) :: x
        double precision, dimension(2), intent(out) :: y
          integer :: i,k

C$openad INDEPENDENT(x)

          do i=1,2
          k=2
            call foo(k,x)
          k = -19
          end do
          y=x
C$openad DEPENDENT(y)
      end subroutine
