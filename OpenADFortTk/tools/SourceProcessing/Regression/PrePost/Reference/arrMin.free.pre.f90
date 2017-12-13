module OAD_intrinsics
  interface oad_s_min
    module procedure oad_s_min_r_11
  end interface
  contains
    subroutine oad_s_min_r_11(a0,a1,r)
      real,intent(in),dimension(:) :: a0
      real,intent(in),dimension(:) :: a1
      real,intent(out),dimension(:) :: r
      integer :: i1
      integer :: l1
      integer :: u1
      l1 = lbound(a1,1)
      u1 = ubound(a1,1)
      do i1 = l1,u1
      if (a0(i1)<a1(i1)) then
        r(i1) = a0(i1)
      else
        r(i1) = a1(i1)
      end if
      end do
    end subroutine
end module
module m
use OAD_intrinsics
contains
subroutine foo(x,y)
  real , dimension(:) :: x,y
  real,dimension(size(x,1)) :: oad_ctmp0
  call oad_s_min(x,y,oad_ctmp0)
  y = oad_ctmp0
end subroutine
end module

program main
 use OAD_intrinsics
 use m
 real :: x(3), y(3)
 x=[1.0,2.0,3.0]
 y=[1.0,1.5,3.5]
 call foo(x,y)
 print *,y
end program
