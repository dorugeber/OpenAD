module m
contains
subroutine foo(x)
  implicit none
  real::x 
  real:: l_x
  l_x=x
  call bar() 
  x=l_x

  contains 
    subroutine bar()
      implicit none
      l_x=l_x*2 ! x from parent foo
    end subroutine 
end subroutine
end module 

program p 
  use m 
  implicit none
  real :: r
  r=3.0 
  call foo(r)
  print *,r
end program
