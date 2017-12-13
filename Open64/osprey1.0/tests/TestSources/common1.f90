! test for plain common block unparsing
program p 
  common /cb/ a,b,c
  real :: a,b,c
  a=1.0
  b=2.0
  c=3.0
  if (b.eq.2.0) then 
    print *, 'OK'
  else 
    print *, b
  end if
end program
