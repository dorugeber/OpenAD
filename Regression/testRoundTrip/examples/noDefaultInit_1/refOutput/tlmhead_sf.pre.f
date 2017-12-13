!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine foo(x,y)
        use OAD_intrinsics
        double precision, dimension(2),intent(in) :: x
        double precision, dimension(1),intent(out) :: y
        double precision :: t
        t = x(1)*x(2)
        y(1)=t
      end subroutine

      subroutine head(x,y)
      use OAD_intrinsics
      interface
          subroutine foo(x,y)
            double precision, dimension(2),intent(in) :: x
            double precision, dimension(1),intent(out) :: y
          end subroutine
        end interface
        double precision, dimension(2) :: x
        double precision, dimension(1) :: y
C$openad INDEPENDENT(x)
        call foo(x,y)
C$openad DEPENDENT(y)
       end subroutine
