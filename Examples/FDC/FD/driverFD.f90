program driver

  implicit none 

  external head

  external init

  double precision, dimension(:), allocatable :: x0
  double precision, dimension(:,:), allocatable :: res_dd
  double precision, dimension(:), allocatable :: x, xph
  double precision, dimension(:), allocatable :: y, yph
  real h
  integer n,m,nx1,nx2
  integer i,j,k
  double precision::r

  h=0.00001

  nx1=5
  nx2=5

  n=nx1*nx2
  m=n

  r=10.0

  allocate(x0(n))
  allocate(res_dd(m,n))
  allocate(x(n))
  allocate(xph(n))
  allocate(y(m))
  allocate(yph(m))

  call init(nx1,nx2,x0)

  write(*,*) "divided differences"
  do i=1,n   
     do j=1,n   
        x(j)=x0(j)
        if (i==j) then 
           xph(j)=x0(j)+h
        else
           xph(j)=x0(j)
        end if
     end do
     call head(nx1,nx2,xph,yph,r)
     call head(nx1,nx2,x,y,r)
     do k=1,m
        res_dd(k,i)=(yph(k)-y(k))/h
     end do
  end do
  do k=1,n
     do i=1,m   
        write(*,'(2(A,I2),A,EN26.16E3)') "F(",i,",",k,")=",res_dd(i,k)
     end do
  end do

  deallocate(x0)
  deallocate(res_dd)
  deallocate(x)
  deallocate(xph)
  deallocate(y)
  deallocate(yph)

end program driver
