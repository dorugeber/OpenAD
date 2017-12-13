module m 

interface foo
module procedure foo_s, foo_a 
end interface 

contains 
function foo_s(la) 
  real :: la,foo_s
  foo_s=la
end function 

function foo_a(lb)
  real, dimension(:):: lb
  real :: foo_a
  foo_a=sum(lb)
end function 

end module 

program main 
  use m 
  real :: a,b(3),fs,fa
  a=1.0
  b=[2.0,3.0,4.0]
  fs=foo(a)
  fa=foo(b)
  print *,fs,fa
end program 
