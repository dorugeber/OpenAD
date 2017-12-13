program driver
  use OAD_active
  use OAD_rev
  implicit none 
  external head
  integer, parameter :: n=2
  type(active) , dimension (n) :: x, b
  double precision, dimension (n,n) :: Jacobian
  integer :: i
  b%v=(/.5D0,12.0D0/)
  print *, 'driver running for b =',b%v
  do i=1,n
     x%d=.0D0
     b%d=.0D0
     x(i)%d=1.0D0
     call OAD_revTape()
     call OpenAD_head(b,x) ! head was renamed
     call OAD_revAdjoint()
     call OpenAD_head(b,x)
     Jacobian(i,:)=b%d
  end do
  write(*,'(A,4(E24.13))') "AD Jacobian: ", Jacobian
end program driver
