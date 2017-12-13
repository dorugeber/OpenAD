module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics
contains
  function foo (s)
    real :: s(:)
    real, dimension(size(s)-1) :: foo
    foo(1:size(s)-1) = s(1:size(s)-1)
  end function
  subroutine oad_s_foo(s,foo)
    real :: s(:)
    real,dimension(size(s)-1),intent(out) :: foo
    foo(1:size(s)-1)=s(1:size(s)-1)
  end subroutine oad_s_foo
end

program p
  use OAD_active
  use OAD_intrinsics
  use m
  real :: i(3)
  real,dimension(size(i)-1) :: oad_ctmp0
  i=(/1,2,3/)
  call oad_s_foo(i,oad_ctmp0)
  write(*,*) oad_ctmp0
end
