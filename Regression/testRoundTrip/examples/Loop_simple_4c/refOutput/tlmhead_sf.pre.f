!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module aGlobalModule
        public :: globalString
        character(3) globalString
      end module

C$openad XXX Template ad_template.f
      subroutine foo(x,y)
        use OAD_intrinsics
        use aGlobalModule
        double precision, dimension(2), intent(in) :: x
        double precision, dimension(2), intent(inout) :: y
        character(3) localString
          integer i
C$openad xxx simple loop
          do i=1,2
          if (globalString.eq.'yes') then
              y(i) = y(i)+x(i)*x(i)
            end if
          if (globalString.eq.'no') then
              y(i) = y(i)+x(i)*x(i)*2
            end if
          end do
        globalString='either'
        localString=globalString
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        use aGlobalModule
        double precision, dimension(2), intent(in) :: x
        double precision, dimension(2), intent(out) :: y
C$openad INDEPENDENT(x)
        y(1)=1.0
        y(2)=1.0
        globalString='yes'
          call foo(x,y)
        globalString='both'
C$openad DEPENDENT(y)
      end subroutine
