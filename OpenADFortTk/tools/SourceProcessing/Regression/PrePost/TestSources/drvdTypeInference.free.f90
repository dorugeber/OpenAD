function foo(i)
  real :: i,foo
  foo=2*i
end function

program p 
  type t 
   real :: i 
  end type
  real y
  type(t) :: aT
  aT%i=2.0
  y=foo(aT%i+2.0)
  print *,y
end program
