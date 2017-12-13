program p 
  common /cb/ cb1,cb2
  real cb1,cb2,f1,f2
  equivalence (cb1,f1)
  equivalence (cb2,f2)
  cb1=1.0
  cb2=2.0
  if (f2.gt.1.5) then 
    print *, 'OK'
  else
    print *, f2
  end if
end program
