module m 

 type t
   real :: x
   real :: y(2)
 end type

end module 

subroutine foo(x,y)
  use m
  real :: x,y
  type(t) :: aT(2)
  aT(1)%x=x
  aT(2)%y=aT(1)%x
  y=aT(2)%y(1)
end subroutine

program p 
  real x,y
  x=2.0
  call foo(x,y)
  print *,y
end program 
