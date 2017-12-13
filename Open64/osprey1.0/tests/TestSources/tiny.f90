program test
! test handling of tiny
  double precision :: x1=tiny(1.0)
! reference x1
  x1=x1+1.0D0
  print *, 'OK'
end 
