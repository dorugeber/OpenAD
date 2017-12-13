program print1
  double precision :: x,y,p
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  p=1.0D0
  x=2.0D0
  y=x
  print *,x,y
  if (y.eq.2.0) then 
    print *, 'OK' 
  end if
end program
