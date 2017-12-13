program driver

  use OAD_active
  use OAD_tape
  use OAD_rev
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
  integer maxDoubleTapeLength
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

  call tape_init()
  write(*,*) "adjoint model, split mode"
  do i=1,m   
     do j=1,n   
        x(j)%v=x0(j)
        x(j)%d=0.0
     end do
     do j=1,m   
        if (i==j) then 
           y(j)%d=1.0
        else
           y(j)%d=0.0
        end if
     end do
     call OAD_revTape()
     call head(nx1,nx2,x,y,r)
     maxDoubleTapeLength=double_tape_pointer-1 ! to illustrate the tape size
     call OAD_revAdjoint()
     call head(nx1,nx2,x,y,r)
     do k=1,n
        res_ad(i,k)=x(k)%d
     end do
  end do
  do k=1,n
     do i=1,m   
        write(*,'(2(A,I2),A,EN26.16E3)') "F(",i,",",k,")=",res_ad(i,k)
     end do
  end do
  write (*,'(2(A,I4))') 'tape stats: doubles ',maxDoubleTapeLength
  deallocate(x0)
  deallocate(res_ad)
  deallocate(x)
  deallocate(y)

end program driver
