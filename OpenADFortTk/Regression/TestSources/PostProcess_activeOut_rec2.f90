module m 
  contains 

  subroutine foo(fooIn,fooOut) 
    double precision :: fooIn, fooOut
    intent (out) :: fooOut
    intent (in) :: fooIn
    fooOut=fooIn
  end subroutine
end module 

program p 
  use m
  double precision:: x,y,t(2),pOut,pIn
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  x=1
  t(1)=x
  do i=1,2
  pIn=t(1) 
  pOut=t(2)
  call foo(pIn,pOut)
  t(2)=pOut
  t(1)=pIn
  end do
  y=t(2)
  print *,y
end program


