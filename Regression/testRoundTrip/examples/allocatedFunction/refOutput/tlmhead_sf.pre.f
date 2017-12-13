!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
        subroutine head(x,y)
          use OAD_intrinsics
          implicit none
          double precision,dimension(2) :: x
          double precision,dimension(1) :: y
          double precision,dimension(:),allocatable :: a
C$openad INDEPENDENT(x)
          allocate(a(2))
          a(1) = x(1)*2
          a(2) = x(2)*2
          if(allocated(a)) then
            y(1) = a(1)*a(2)
          else
            y(1) = 0
          endif
          deallocate(a)
C$openad DEPENDENT(y)
        end subroutine head
