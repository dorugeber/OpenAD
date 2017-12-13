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
        double precision, dimension(1), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
          integer(8) :: i
C$openad INDEPENDENT(x)
C transfer is a passivating operation !
          i=transfer(x(1),i)
        y(1) = x(1)+transfer(i,x(1))
C$openad DEPENDENT(y)
      end subroutine
