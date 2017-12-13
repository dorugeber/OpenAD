function foo(i)
  real, dimension(2) :: i,foo
  foo=2*i
end function

program p 
  interface
     function foo(i)
       real, dimension(2) :: i,foo
     end function foo
  end interface
  type t 
   real, dimension(2,2) :: i 
   integer :: idx=2
  end type
  real y(2)
  type(t) :: aT
  aT%i=2.0
  y=foo(aT%i(aT%idx,:)*2)
  print *,y
end program
