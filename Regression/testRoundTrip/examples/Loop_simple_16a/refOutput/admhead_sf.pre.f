!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(x,y,l,u,s)
        use OAD_intrinsics
        double precision, dimension(5), intent(in) :: x
        double precision, intent(inout) :: y
          integer i,l,u,s
C$openad xxx simple loop
          do i=l,u,s
            if (i.lt.5) then
              y = y*x(i)
            end if
          end do
      end subroutine


C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(5), intent(in) :: x
        double precision, dimension(4), intent(out) :: y
        integer :: oad_ctmp0
        integer :: oad_ctmp1
        integer :: oad_ctmp2
        integer :: oad_ctmp3
        integer :: oad_ctmp4
        integer :: oad_ctmp5
        integer :: oad_ctmp6
        integer :: oad_ctmp7
        integer :: oad_ctmp8
        integer :: oad_ctmp9
        integer :: oad_ctmp10
        integer :: oad_ctmp11
C$openad INDEPENDENT(x)
          y=x(1:4)
        oad_ctmp0 = 1
        oad_ctmp1 = 3
        oad_ctmp2 = 1
        call foo(x,y(1),oad_ctmp0,oad_ctmp1,oad_ctmp2)
        oad_ctmp3 = 3
        oad_ctmp4 = 1
        oad_ctmp5 = -1
        call foo(x,y(2),oad_ctmp3,oad_ctmp4,oad_ctmp5)
        oad_ctmp6 = 1
        oad_ctmp7 = 5
        oad_ctmp8 = 2
        call foo(x,y(3),oad_ctmp6,oad_ctmp7,oad_ctmp8)
        oad_ctmp9 = 5
        oad_ctmp10 = 1
        oad_ctmp11 = -2
        call foo(x,y(4),oad_ctmp9,oad_ctmp10,oad_ctmp11)
C$openad DEPENDENT(y)
      end subroutine
