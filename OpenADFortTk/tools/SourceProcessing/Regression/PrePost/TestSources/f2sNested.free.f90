module m
implicit none

type t
  real, dimension(2) :: a
end type

contains 

function bar(r)
  real :: r,bar
  bar=2*r
end function

function foo(aT)
  type(t) :: aT
  real :: foo
  foo=bar(aT%a(2))
end function

end module

program p 
  use m 
  type(t) :: aT
  real y
  aT%a(1)=3.2
  aT%a(2)=4.1
  y=foo(aT) 
  print *,y
end program
