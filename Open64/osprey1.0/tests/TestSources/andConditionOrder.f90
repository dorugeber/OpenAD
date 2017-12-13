program arrayAnd

  integer :: a, b(2)
  a=10000
  b(1)=0
  b(2)=2
! mfef90 flips the order of the condition
! to  (b(a)==0 and a<3) 
! and we get ouf of bounds array access
  if ( a<3 .and. b(a)==0) then 
     print *,'FAILED'
  end if
  print *,'OK'
end program arrayAnd
