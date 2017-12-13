function foo(i)
  real :: i,foo
  foo=2*i
end function

program p 
  type t 
   real, dimension(2) :: i 
  end type
  real y
  type(t) :: aT
  aT%i(1)=2.0
  y=foo(aT%i(1)+2.0)
  print *,y
end program
