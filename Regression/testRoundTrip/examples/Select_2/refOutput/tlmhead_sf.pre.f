!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]

C -------------------------------------------------------
C NOTE: THIS EXAMPLE SEGVS IN REVERSE BECAUSE OF BUG #78
C -------------------------------------------------------
C
C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(3), intent(in) :: x
        double precision, dimension(3), intent(out) :: y
          integer i
C$openad INDEPENDENT(x)
          do i=1,3
          select case (i)
          case (1)
            y(i)=sin(x(i))
          case (2)
            y(i)=cos(x(i))
          case default
            y(i) = 2*x(i)
          end select
        end do
C$openad DEPENDENT(y)
      end subroutine
