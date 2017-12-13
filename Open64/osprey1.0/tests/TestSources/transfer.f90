program transp
  integer, dimension(2,2) :: A
  integer, dimension(4) :: B
  data A /1,2,3,4/
  B = transfer(A,B)
  if (B(3).EQ.3) then 
    print *,'OK'
  else 
    print *, B,A
  end if
end program

