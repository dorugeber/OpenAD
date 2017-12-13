module m1 
implicit none
type t1
 integer t1i
end type 
type t2
 integer t2i
end type 

interface foo
  module procedure foot1, foot2
end interface

contains 
  function foot1(a1)
    type(t1) :: a1
    integer foot1
    foot1=a1%t1i
  end function 
  function foot2(a2)
    type(t2) :: a2
    integer foot2
    foot2=a2%t2i
  end function
end module 

subroutine head(i)
  use m1
  implicit none
  type(t1):: at1
  integer i
  at1%t1i=3
  i=foo(at1)
end subroutine 

program p 
   integer i 
  call head(i)
  print *,i
end program
