program transp
  integer, dimension(2,2) :: A,B
  data A /1,2,3,4/
  B = transpose(A)
  if (B(1,2).EQ.2) then 
    print *,'OK'
  else 
    print *, B,A
  end if
end program

