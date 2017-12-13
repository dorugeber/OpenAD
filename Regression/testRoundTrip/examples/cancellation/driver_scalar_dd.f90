program driver

  implicit none 

  external head

  ! inputs
  double precision x1, x2
  double precision x_ph1, x_ph2

  ! outputs
  double precision y
  double precision y_ph

  double precision, dimension(2) :: x0
  double precision :: h
  integer:: n,m
  integer i,j,k

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)

  do i=1,n   
     x0(i)=sqrt(1.*i)
  end do

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  x1=x0(1)
  x2=x0(2)

  call head (x1, x2, y)

  do i=1,n   
     x_ph1=x0(1)
     x_ph2=x0(2)

     if (i==1) x_ph1=x_ph1+h
     if (i==2) x_ph2=x_ph2+h
     call head (x_ph1, x_ph2, y_ph)
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",i,")=",(y_ph-y)/h
  end do
  close(2)

end program driver
