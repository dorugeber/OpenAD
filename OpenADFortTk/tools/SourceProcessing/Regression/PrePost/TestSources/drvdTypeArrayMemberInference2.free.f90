program p 
  type t 
   real :: i(2) 
  end type
  real y
  type(t) :: aT(2)
  aT(1)%i(1)=2.0
  write (*,*) aT(1)%i(1) 
end program
