program p 
  real v1,v2,f1,f2
  equivalence (v1,f1)
  equivalence (v2,f2)
  v1=1.0
  v2=2.0
  if (f2.gt.1.5) then 
    print *, 'OK'
  else
    print *, f2
  end if
end program
