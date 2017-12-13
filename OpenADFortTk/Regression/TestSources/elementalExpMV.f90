! elemental exp

program arrays
  double precision :: x(2,2),y(2)
  DATA x /1.0,2.0,3.0,4.0/
  y=exp(x(1,:))
  print *,y

end program
