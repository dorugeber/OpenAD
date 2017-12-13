module m1
 contains 
  real function foo(x)
    real :: x 
    foo=x*2
    print *, "foo ", x
  end function 
  double precision function bar(y)
    double precision :: y 
    bar=y*2
    print *, "bar ", y
  end function 
end module 

module m2
  use m1 
  interface i 
   module procedure foo
   module procedure bar
  end interface
end module 

subroutine head(x,y)
  use m2
  real :: x
  double precision :: t,y
  x=i(x)
  t=x
  t=i(t)
  y=t
end subroutine 

program p 
  real ::  x 
  double precision :: y 
  x=1.0
  call head(x,y)
  print *,y
end program

