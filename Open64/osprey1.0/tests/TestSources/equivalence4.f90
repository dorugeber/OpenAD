program p 
  real v1,v2,f1,f2,x
  equivalence (v1,f1)
  equivalence (v2,f2)
  v1=1.0
  v2=2.0
  x=3.0
  if (f2.gt.1.5 .and. f1.lt.1.5) then 
    print *, 'OK'
  else
    print *, f1,f2,x
  end if
end program
