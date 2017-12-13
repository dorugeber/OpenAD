program driver

  use OAD_active
  implicit none 

  external head

  double precision x0
  type(active):: x,y,y0
  double precision h

  x0 = 0.5D0
  h=0.0001

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  call head(x0,y0)
  x%v = x0+h
  call head(x,y)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",1,")=",(y%v-y0%v)/h
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  x%v = x0
  x%d(1) = 1.0D0
  call head(x,y)
  write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",1,")=",y%d(1)
  close(2)
end program driver

