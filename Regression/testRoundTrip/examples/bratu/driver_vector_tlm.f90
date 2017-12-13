program driver

  use OAD_active
  implicit none 

  external head

  double precision, dimension(:), allocatable :: x0
  type(active), dimension(:), allocatable :: x, xph
  type(active), dimension(:), allocatable :: y, yph
  double precision h
  integer n,m
  integer i,j,k

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)


  allocate(x0(n))
  allocate(x(n))
  allocate(xph(n))
  allocate(y(m))
  allocate(yph(m))

  x0(1) = 1.72
  x0(2) = 3.45
  x0(3) = 4.16
  x0(4) = 4.87
  x0(5) = 4.16
  x0(6) = 3.45
  x0(7) = 1.72
  x0(8) = 1.3
  x0(9) = 0.245828 

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  do i=1,n   
     do j=1,n   
        x(j)%v=x0(j)
        if (i==j) then 
           xph(j)%v=x0(j)+h
        else
           xph(j)%v=x0(j)
        end if
     end do
     call head(xph,yph)
     call head(x,y)
     do k=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",k,",",i,")=",(yph(k)%v-y(k)%v)/h
     end do
  end do
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  do i=1,n   
     x(i)%v=x0(i)
     do j=1,n   
        if (i==j) then 
           x(i)%d(j) = 1.0D0
        else
           x(i)%d(j) = 0.0D0
        end if
     end do
  end do
  call head(x,y)
  do i=1,n   
     do k=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",k,",",i,")=",y(k)%d(i)
     end do
  end do
  close(2)

  deallocate(x0)
  deallocate(x)
  deallocate(xph)
  deallocate(y)
  deallocate(yph)

end program driver


