function foo(i)
  real :: i,foo
  foo=2*i
end function

program p 
  type t 
   real :: i 
  end type
  type tt
   type(t) :: aT
  end type
  real y
  type(tt) :: aTT
  aTT%aT%i=2.0
  y=foo(aTT%aT%i+2.0)
  print *,y
end program
