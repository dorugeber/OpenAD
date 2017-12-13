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
        double precision, dimension(2), intent(inout) :: x
        double precision, dimension(2), intent(inout) :: y
        double precision, dimension(2,2) :: A,B
C$openad INDEPENDENT(x)
          A=0.0
        A(1,1) = x(1)
        A(1,2) = x(2)
        B = transpose(A)
        y(1) = b(1,1)
        y(2) = b(2,1)
C$openad DEPENDENT(y)
      end subroutine

