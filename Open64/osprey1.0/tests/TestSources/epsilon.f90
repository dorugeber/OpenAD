program test
! test handling of epsilon
  double precision :: x1=epsilon(1.0)
! reference x1
  x1=x1+1.0D0
  print *, 'OK'
end 
