subroutine bar()
  implicit none

  save /cb/
  common /cb/ G
  type(oadactive) :: G
  
end subroutine bar
