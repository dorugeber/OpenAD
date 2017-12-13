program driver

  use OAD_active
  implicit none 

  external head

  external init

  double precision, dimension(:), allocatable :: x0
  double precision, dimension(:,:), allocatable :: res_ad
  type(active), dimension(:), allocatable :: x
  type(active), dimension(:), allocatable :: y
  integer n,m,nx1,nx2
  integer i,j,k
  type(active)::r

  nx1=5
  nx2=5

  n=nx1*nx2
  m=n

  r%v=10.0

  allocate(x0(n))
  allocate(res_ad(m,n))
  allocate(x(n))
  allocate(y(m))

  call init(nx1,nx2,x0)

  write(*,*) "tangent linear model"
  do i=1,n   
     do j=1,n   
        x(j)%v=x0(j)
        if (i==j) then 
           x(j)%d=1.0
        else
           x(j)%d=0.0
        end if
     end do
     call head(nx1,nx2,x,y,r)
     do k=1,m
        res_ad(k,i)=y(k)%d
     end do
  end do
  do k=1,n
     do i=1,m   
        write(*,'(2(A,I2),A,EN26.16E3)') "F(",i,",",k,")=",res_ad(i,k)
     end do
  end do

  deallocate(x0)
  deallocate(res_ad)
  deallocate(x)
  deallocate(y)

end program driver
