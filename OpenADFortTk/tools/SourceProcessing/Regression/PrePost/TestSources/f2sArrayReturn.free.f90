module m 
contains
  function foo (s) 
    real :: s(:)
    real, dimension(size(s)-1) :: foo
    foo(1:size(s)-1)=s(1:size(s)-1)
  end function
end

program p 
  use m 
  real :: i(3)
  i=(/1,2,3/)
  write (*,*) foo(i)
end  
