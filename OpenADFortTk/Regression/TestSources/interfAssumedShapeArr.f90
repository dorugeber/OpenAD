module m
  interface 
    subroutine foo(i)
      integer , dimension(*) :: i 
    end subroutine 
  end interface 
end module 

subroutine foo(i)
  integer , dimension(*) :: i
  print *,i(2)
end subroutine

program p 
  use m 
  integer i(2)
  i(1)=1
  i(2)=2
  call foo(i)
end program
