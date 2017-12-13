module m 

interface foo
module procedure foo_s, foo_d 
end interface 

contains 
function foo_s(a,b) 
  real :: a,b,foo_s
  foo_s=a+b
end function 

function foo_d(c,d)
  double precision:: c,d,foo_d
  foo_d=c-d
end function 

end module 

program main 
  use m 
  real(kind(1.0D0)) :: c,d,fd
  real(kind(0.0)) :: a,b,fs
  a=1.0
  b=2.0
  fs=foo(a,b)
  c=1.0D0
  d=2.0D0
  fd=foo(c,d)
  print *,fs,fd
end program 
