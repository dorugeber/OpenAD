program driver

  implicit none 

  external head

  external init

  double precision, dimension(:), allocatable :: x
  double precision, dimension(:), allocatable :: y
  integer n,m,nx1,nx2
  integer k
  double precision ::r

  nx1=5
  nx2=5

  n=nx1*nx2
  m=n

  r=10.0

  allocate(x(n))
  allocate(y(m))

  call init(nx1,nx2,x)
  call head(nx1,nx2,x,y,r)

  do k=1,m
     write(*,'(A,I2,A,EN26.16E3)') "y(",k,")=",y(k)
  end do

  deallocate(x)
  deallocate(y)

end program driver
