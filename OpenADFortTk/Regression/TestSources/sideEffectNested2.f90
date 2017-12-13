module m
contains
subroutine foo(x)
  implicit none
  real::x 
  call bar() 

  contains 
    subroutine bar()
      implicit none
      x=x*2 ! x from parent foo
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
