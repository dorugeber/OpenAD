program driver
  implicit none 
  external head
  integer, parameter :: n=2
  double precision , dimension (n) :: x, b, xh, bh
  double precision , dimension (n,n) :: Jacobian
  double precision :: h=0.00001D0
  integer :: i,j
  b=(/.5D0,12.0D0/)
  call head(b,x)
  do i =1,n
   bh=b
   bh(i)=bh(i)+h
   call head(bh,xh)
   Jacobian(:,i)=(xh-x)/h
  end do
  write(*,'(A,4(E24.13))') "FD Jacobian: ", Jacobian
end program driver
