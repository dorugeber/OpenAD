program rType
  double precision :: s, a(2,2)
  data a /1.0D0,2.0D0,3.0D0,4.0D0/
  s=sin(a(1,1))
  print *,s
end program
