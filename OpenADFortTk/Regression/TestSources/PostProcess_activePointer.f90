!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with an array pointer variable
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program arrayPointerShape
  double precision, pointer :: btn
  double precision, target :: x,y
!$openad INDEPENDENT(x)
  btn=>x
  x=1.0D0
  y=btn
  print *, y
  print *, 'OK'
!$openad DEPENDENT(y)
end program

