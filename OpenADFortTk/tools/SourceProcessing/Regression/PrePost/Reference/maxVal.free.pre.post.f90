module OAD_intrinsics
  use OAD_active
  interface oad_s_maxval
    module procedure oad_s_maxval_d_1
  end interface
  contains
    subroutine oad_s_maxval_d_1(a,r)
      double precision,intent(in),dimension(:) :: a
      double precision,intent(out) :: r
      integer :: i(1)
        i = maxloc(a)
        r = a(i(1))
    end subroutine
end module
program maxValTest
  use OAD_active
  use OAD_intrinsics
  implicit none
  double precision x(4),y
  double precision :: oad_ctmp0
  x=(/1.0, 2.0, 3.0, 1.5/)
  call oad_s_maxval(x,oad_ctmp0)
  y = oad_ctmp0
  write(*,*) 'y = ',y
end program maxValTest

