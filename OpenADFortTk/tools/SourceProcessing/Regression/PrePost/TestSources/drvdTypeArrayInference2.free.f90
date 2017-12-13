function foo(i)
  real :: i(2),foo
  foo=2*i(1)
end function

program p 
  type t 
   real, dimension(2) :: i 
  end type
  type tt
   type (t) :: aT
  end type
  real y
  type(tt) :: aTT
  aTT%aT%i(1)=2.0
  y=foo(aTT%aT%i(:)+2.0)
  print *,y
end program
