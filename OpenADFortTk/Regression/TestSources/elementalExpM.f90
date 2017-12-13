! elemental exp

program arrays
  double precision, dimension(2,2) :: x,y
  DATA x /1.0,2.0,3.0,4.0/
  y=exp(x(:,:))
  print *,y

end program
