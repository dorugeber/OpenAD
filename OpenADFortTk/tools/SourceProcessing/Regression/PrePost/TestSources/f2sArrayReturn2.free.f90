module m 
contains
  function foo (s) 
    real :: s(:,:)
    real, dimension(size(s,1)) :: foo
    do i=1,size(s,1)
      foo(i)=sum(s(:,i))
    end do
  end function
end

program p 
  use m 
  real :: i(2,2,2)
  i=reshape((/1,2,3,4,5,6,7,8/),shape(i))
  do j=1,2
    write (*,*) foo(i(:,:,j))
  end do
end  
