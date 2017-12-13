!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod
      
        public :: g
        
        integer g
        
      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(a,n)
        use OAD_intrinsics
        use all_globals_mod
        double precision :: a(n+g)
        integer n
        integer i
        do i=1,n+1
          a(i) = 2*a(i)
        end do
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        use all_globals_mod
        double precision, dimension(3) :: x
        double precision, dimension(1) :: y
        integer i
        integer :: oad_ctmp0
C$openad INDEPENDENT(x)
        g=1
        i=2
        oad_ctmp0 = 2
        call foo(x,oad_ctmp0)
        call foo(x,i)
        y(1)=0
        do i=1,3
         y(1) = y(1)+x(i)
        end do
C$openad DEPENDENT(y)
      end subroutine
