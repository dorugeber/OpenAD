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

module m2
contains 
subroutine foo2(x)
  use m
  implicit none
  real :: x
  call foo(x)
end subroutine
end module

program p 
  use m2 
  implicit none
  real :: r
  r=3.0 
  call foo2(r)
  print *,r
end program
