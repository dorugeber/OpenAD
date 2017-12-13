!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module m
        double precision, allocatable :: t(:)
      end module

      subroutine foo(t)
      use OAD_intrinsics
C$openad XXX Template ad_template.f
        double precision :: t(:)
        print *,shape(t)
        call bar()
        print *,shape(t)
      end subroutine

      subroutine bar()
C$openad XXX Template ad_template.f
        use OAD_intrinsics
        use m
        deallocate(t)
          allocate(t(3))
          do i=1,3
             t(i) = 1.0D0+i*.5D0
        end do
      end subroutine

      subroutine head(x,y)
C$openad XXX Template ad_template.f
        use OAD_intrinsics
        use m
        interface
           subroutine foo(t)
           double precision :: t(:)
           end subroutine
        end interface
        double precision, dimension(1) :: x,y
C$openad INDEPENDENT(x)
        allocate(t(2))
          t(2)=x(1)
          t(2) = t(2)*2
          x(1)=t(2)
        call foo(t)
        y(1) = x(1)*t(3)
        deallocate(t)
C$openad DEPENDENT(y)
      end subroutine

