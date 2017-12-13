!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with an array pointer variable
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program arrayPointerShape
  real, dimension (:,:), pointer :: btn

  allocate(btn(1,1))
  btn(1,1) = 5.5
  print *, btn(1,1)
  print *, 'OK'
end program

