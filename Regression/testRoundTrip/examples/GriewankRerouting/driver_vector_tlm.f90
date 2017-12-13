program driver

  use OAD_active
  implicit none 

  external ad_reroute

  ! inputs
  type(active) x1,x2

  ! outputs
  type(active) y01,y02,y1,y2

  double precision, dimension(2) :: x0
  double precision :: h
  integer:: n,m
  integer i

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n,m,h
  close(2)

  do i=1,n
     x0(i) = sqrt(1.0D0*(i+1))
  end do

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  call ad_reroute(x0(1),x0(2),y01,y02)
  x1%v = x0(1)
  x2%v = x0(2)
  do i=1,n
     x1%v = x0(1)
     x2%v = x0(2)
     if (i==1) x1%v = x0(1)+h
     if (i==2) x2%v = x0(2)+h
     call ad_reroute(x1,x2,y1,y2)
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",i,")=",(y1%v-y01%v)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",i,")=",(y2%v-y02%v)/h
  end do
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  x1%v = x0(1)
  x1%d(1) = 1.0D0
  x1%d(2) = 0.0D0
  x2%v = x0(2)
  x2%d(1) = 0.0D0
  x2%d(2) = 1.0D0
  call ad_reroute(x1,x2,y1,y2)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",1,")=",y1%d(1)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",1,")=",y2%d(1)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",2,")=",y1%d(2)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",2,")=",y2%d(2)
  close(2)

end program driver
