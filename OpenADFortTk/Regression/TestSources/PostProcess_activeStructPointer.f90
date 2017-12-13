!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with an array pointer variable
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
module globalStructure
  implicit none

  type :: model_type
    real, pointer :: ptr
  end type model_type
  type(model_type), target, save :: clm3

end module globalStructure

program arrayPointerShape
  use globalStructure
  implicit none

  real, pointer :: btn
  !real, pointer :: loc
  real, target :: x,y
!$openad INDEPENDENT(x)

  x=1.0
  clm3%ptr => x

  !btn => clm3%ptr
  y=x
  print *, y
  print *, 'OK'
!$openad DEPENDENT(y)
end program

