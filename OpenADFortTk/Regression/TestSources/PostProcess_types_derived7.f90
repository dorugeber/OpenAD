module m 

 type t
   real :: x
   real,pointer,dimension(:) :: y
 end type

end module 

subroutine foo(x,y)
  use m
  real :: x,y
  type(t),dimension(:),allocatable :: aT
  allocate(aT(1))
  aT(1)%x=x
  allocate ( aT(1)%y(1) )
  aT(1)%y(:)=0.0
  y=aT(1)%y(1)
end subroutine

program p 
  real x,y
  x=2.0
  call foo(x,y)
  print *,y
end program 
