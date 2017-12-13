program driver

  use OAD_active
  implicit none 

  external head

  double precision,dimension(2) :: x0
  type(active) :: x1,x2,y1,y2,y1ph,y2ph
  double precision :: h
  integer n,m

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n,m,h
  close(2)

  x0(1) = 1.0D0
  x0(2) = 2.0D0

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  x1%v = x0(1)
  x2%v = x0(2)
  call head(x1,x2,y1,y2)

  x1%v = x0(1)+h
  x2%v = x0(2)
  call head(x1,x2,y1ph,y2ph)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",1,")=",(y1ph%v-y1%v)/h
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",1,")=",(y2ph%v-y2%v)/h

  x1%v = x0(1)
  x2%v = x0(2)+h
  call head(x1,x2,y1ph,y2ph)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",2,")=",(y1ph%v-y1%v)/h
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",2,")=",(y2ph%v-y2%v)/h
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  x1%v = x0(1)
  x1%d(1) = 1.0D0
  x1%d(2) = 0.0D0
  x2%v = x0(2)
  x2%d(1) = 0.0D0
  x2%d(2) = 1.0D0
  call head(x1,x2,y1,y2)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",1,")=",y1%d(1)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",1,")=",y2%d(1)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",2,")=",y1%d(2)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",2,")=",y2%d(2)
  close(2)

end program driver

