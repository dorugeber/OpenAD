!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with an array pointer variable
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program arrayPointerShape
  double precision, dimension (:,:), pointer :: btn
  double precision :: x,y
!$openad INDEPENDENT(x)
  allocate(btn(1,1))
  x=1.0D0
  btn(1,1)=x
  y=btn(1,1)
  print *, y
  print *, 'OK'
!$openad DEPENDENT(y)
end program

