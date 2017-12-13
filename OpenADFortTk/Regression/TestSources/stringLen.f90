program stringLen

  double precision a
  character(10) s
  s='bla'
  a=2*len(trim(s))+1.0D0
  print *,a
end program
