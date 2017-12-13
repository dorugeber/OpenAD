!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(1), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
        integer :: i,j,ii
C$openad INDEPENDENT(x)
        do j = 1, 5
          do ii = 1, 5
            exit
          end do
          do ii = 1, 5
            y(1) = x(1)
          end do
        end do
        return
C$openad DEPENDENT(y)
      end subroutine head
