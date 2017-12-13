!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      subroutine foo(x)
        use OAD_intrinsics
        double precision :: x
        x = x+1.0
      end subroutine
      
      subroutine bar(x)
        use OAD_intrinsics
        double precision :: x(1)
        x(1) = x(1)+1.0
      end subroutine
      
      subroutine head(x,y)
        use OAD_intrinsics
        interface
           subroutine foo(x)
             double precision :: x
           end subroutine foo
        end interface
        interface
           subroutine bar(x)
             double precision :: x(1)
           end subroutine bar
        end interface
        double precision, dimension(2) :: x,y
C$openad INDEPENDENT(x)
        call foo(x(1))
        y=x
        call bar(y(2:2))
C$openad DEPENDENT(y)
      end subroutine
