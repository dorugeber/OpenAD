function foo(i)
  real :: i,foo
  foo=2*i
end function

program p 
  type t 
   real, dimension(2) :: i 
   integer :: idx=2
  end type
  real y, foo
  type(t) :: aT
  aT%i(1)=2.0
  y=foo(aT%i(aT%idx)*2)
  print *,y
end program
