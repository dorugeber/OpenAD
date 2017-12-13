!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine bar(x,y)
        use OAD_intrinsics
        double precision, dimension(4) :: x,y
        integer :: i
        do i=1,4
           if (x(i)>1.2) then
      	y(i) = 2.0*y(i)
             end if
        end do
      end subroutine

      subroutine foo(x,y)
        use OAD_intrinsics
        double precision, dimension(:) :: x,y
        call bar(x,y)
      end subroutine

      subroutine head(x,y)
      use OAD_intrinsics
      interface
      subroutine foo(x,y)
        double precision, dimension(:) :: x,y
      end subroutine
      end interface
        double precision, dimension(4) :: x,y
C$openad INDEPENDENT(x)
        y=x
        call foo(x,y)
C$openad DEPENDENT(y)
      end subroutine

