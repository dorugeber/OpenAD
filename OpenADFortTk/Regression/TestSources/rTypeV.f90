program rType
  double precision :: a(2,2)
  data a /1.0D0,2.0D0,3.0D0,4.0D0/
  a=transpose(a)
  print *,a
end program
