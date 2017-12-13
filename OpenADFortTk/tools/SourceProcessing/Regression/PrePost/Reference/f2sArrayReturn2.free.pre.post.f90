module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics
contains
  function foo (s)
    real :: s(:,:)
    real, dimension(size(s,1)) :: foo
    do i = 1,size(s,1)
      foo(i) = sum(s(:,i))
    end do
  end function
  subroutine oad_s_foo(s,foo)
    real :: s(:,:)
    real,dimension(size(s,1)),intent(out) :: foo
    do i=1,size(s,1)
      foo(i)=sum(s(:,i))
    end do
  end subroutine oad_s_foo
end

program p
  use OAD_active
  use OAD_intrinsics
  use m
  real :: i(2,2,2)
  real,dimension(size(i(:,:,:),1)) :: oad_ctmp0
  i=reshape((/1,2,3,4,5,6,7,8/),shape(i))
  do j=1,2
    call oad_s_foo(i(:,:,j),oad_ctmp0)
    write(*,*) oad_ctmp0
  end do
end
