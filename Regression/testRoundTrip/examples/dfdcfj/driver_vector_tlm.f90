program driver

  use OAD_active
  implicit none 

  external head

  double precision,dimension(:),allocatable :: x0
  type(active), dimension(:), allocatable :: x,xph,y,yph
  double precision h
  integer n,m,nx1,nx2
  integer i,j,k
  double precision :: hx1,hx2,xx2,xx1,r
  double precision one
  parameter(one=1.0d0)

  nx1=5
  nx2=5
  !	  n should nx1*nx2 

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n,m,h
  close(2)

  r=10.0D0

  allocate(x0(n))
  allocate(x(n))
  allocate(xph(n))
  allocate(y(m))
  allocate(yph(m))

  !         initial value 

  hx1 = one/dble(nx1 + 1)
  hx2 = one/dble(nx2 + 1)

  xx2 = hx2
  do i = 1, nx2
     xx1 = hx1
     do j = 1, nx1
        k = (i - 1)*nx1 + j
        x0(k) = - xx1*(one - xx1)*xx2*(one - xx2)
        xx1 = xx1 + hx1
     end do
     xx1 = hx1
     xx2 = xx2 + hx2
  end do

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  do i=1,n
     do j=1,n
        x(j)%v = x0(j)
        if (i==j) then 
           xph(j)%v = x0(j)+h
        else
           xph(j)%v = x0(j)
        end if
     end do
     call head(nx1,nx2,xph,yph,r)
     call head(nx1,nx2,x,y,r)
     do j=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",j,",",i,")=",(yph(j)%v-y(j)%v)/h
     end do
  end do
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  do i=1,n   
     x(i)%v = x0(i)
     do j=1,n   
        if (i==j) then 
           x(i)%d(j) = 1.0D0
        else
           x(i)%d(j) = 0.0D0
        end if
     end do
  end do
  call head(nx1,nx2,x,y,r)
  do i=1,n
     do j=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",j,",",i,")=",y(j)%d(i)
     end do
  end do
  close(2)

  deallocate(x0)
  deallocate(x)
  deallocate(xph)
  deallocate(y)
  deallocate(yph)

end program driver


