program main_g

  implicit none 
  external driver_g
  external head

  double precision, dimension(:), allocatable :: x0
  double precision, dimension(:,:), allocatable :: x, g
  double precision, dimension(2) :: y, y0
  real h
  integer n
  integer i,j

  n=2
  h=0.00001

  allocate(x(n,2))
  allocate(g(n,2))
  allocate(x0(n))

  do i=1,n   
    x0(i)=i*1.0
    x(i,1)=x0(i)
    x(i,2)=0.0
  end do

  write(*,*) 'DD: gradient of function'
  call head(x,y0)
  do i=1,n   
     do j=1,n   
        x(i,2)=0.0
        x(j,1)=x0(j)
        if (i==j) then 
           x(j,1)=x0(j)+h
        end if
     end do
     call head(x,y)
     write(*,'(A,I3,A,E25.17E3)') "g(",i,")=",(y(1)-y0(1))/h
  end do

  do i=1,n
     x(i,1)=x0(i)
     x(i,2)=0.0
  end do

  write(*,*) 'AD: gradient of function'
  call driver_g(x,g)
  do i=1,n   
    write(*,'(A,I3,A,E25.17E3)') "g(",i,")=",g(i,1)
  end do

  deallocate(x)
  deallocate(g)
  deallocate(x0)

end program main_g
