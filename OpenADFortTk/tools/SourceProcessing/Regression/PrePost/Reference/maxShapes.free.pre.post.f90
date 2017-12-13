module OAD_intrinsics
  use OAD_active
  interface oad_s_max
    module procedure oad_s_max_d_01
  end interface
  contains
    subroutine oad_s_max_d_01(a0,a1,r)
      double precision,intent(in) :: a0
      double precision,intent(in),dimension(:) :: a1
      double precision,intent(out),dimension(:) :: r
      integer :: i1
      integer :: l1
      integer :: u1
      l1 = lbound(a1,1)
      u1 = ubound(a1,1)
      do i1 = l1,u1
      if (a0>a1(i1)) then
        r(i1) = a0
      else
        r(i1) = a1(i1)
      end if
      end do
    end subroutine
end module
program maxShapes
  use OAD_active
  use OAD_intrinsics
  implicit none
  double precision x(3),a,b,y
  dimension :: y(3)
  double precision,dimension(3) :: oad_ctmp0
  double precision,dimension(3) :: oad_ctmp1
  a=2.0D0
  b=4.0D0
  x=(/1.0, 2.0, 3.0/)
  oad_ctmp1 = b*(a-x)
  call oad_s_max(a,oad_ctmp1,oad_ctmp0)
  y = oad_ctmp0
  write(*,*) 'y = ',y
end program maxShapes

