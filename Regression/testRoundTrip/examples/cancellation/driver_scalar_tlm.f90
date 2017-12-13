program driver

  use OAD_active
  implicit none 

  external head

  ! inputs
  type(active) x1,x2

  ! outputs
  type(active) y0,y

  double precision, dimension(2) :: x0
  double precision :: h
  integer:: n,m,i

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n,m,h
  close(2)

  do i=1,n   
     x0(i) = sqrt(1.*i)
  end do

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  x1%v=x0(1)
  x2%v=x0(2)

  call head (x1,x2,y0)

  do i=1,n   
     x1%v=x0(1)
     x2%v=x0(2)

     if (i==1) x1%v = x0(1)+h
     if (i==2) x2%v = x0(2)+h
     call head (x1,x2,y)
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",i,")=",(y%v-y0%v)/h
  end do
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  do i=1,n   
     x1%v = x0(1)
     x2%v = x0(2)

     x1%d = 0.0D0
     x2%d = 0.0D0
     if (i==1) x1%d = 1.0D0
     if (i==2) x2%d = 1.0D0
     call head(x1,x2,y)
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",i,")=",y%d
  end do
  close(2)

end program driver
