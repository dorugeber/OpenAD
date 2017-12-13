module OAD_intrinsics
end module
subroutine foo(i)
  use OAD_active
  use OAD_intrinsics
  print *,i
end subroutine

program maxlocTest
  use OAD_active
  use OAD_intrinsics
  implicit none
  double precision x(4),y(2,2)
  integer,dimension(1:1) :: oad_ctmp0
  integer,dimension(1:2) :: oad_ctmp1
  x=(/1.0, 2.0, 3.0, 1.5/)
  y=reshape(x,(/2,2/))
  oad_ctmp0 = maxloc(x)
  call foo(oad_ctmp0)
  oad_ctmp1 = maxloc(y)
  call foo(oad_ctmp1)
  contains
    subroutine foo(i)
      integer :: i(:)
      print *,i
    end subroutine foo
end program

