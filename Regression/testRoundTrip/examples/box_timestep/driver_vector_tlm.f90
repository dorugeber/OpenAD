program driver

  use OAD_active
  implicit none

  external box_timestep

  integer, parameter :: kdim=3
  integer i,j,n,m
  double precision :: h
  double precision jac(kdim,kdim)

  double precision :: gamma_t
  double precision :: nullforce(1 : 2)
  double precision :: tstar(1 : 2)
  double precision :: told(1 : 3)
  double precision :: tnow0(1 : 3)
  type(active) :: tnow(1 : 3)
  double precision :: uvel

  type(active) :: tnew0(1 : 3)
  type(active) :: tnew(1 : 3)

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n,m,h
  close(2)

  gamma_t=1.0D0
  nullforce=(/1.0D0,1.0D0/)
  tstar=(/1.0D0,1.0D0/)
  told=(/1.0D0,1.0D0,1.0D0/)
  tnow0=(/1.0D0,1.0D0,1.0D0/)
  uvel=1.0D0

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  do i = 1,kdim
    tnow(i)%v = tnow0(i)
  end do
  call box_timestep(gamma_t,tStar,nullforce,uvel,tnow,told,tnew0)
  do i=1,kdim
    do j=1,kdim
      tnow(j)%v=tnow0(j)
      if (j==i) then
        tnow(j)%v=tnow0(j)+h
      end if
    end do
    call box_timestep(gamma_t,tStar,nullforce,uvel,tnow,told,tnew)
    do j=1,kdim
      write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",j,")=",(tnew(j)%v-tnew0(j)%v)/h
    end do
  end do
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  do i=1,kdim
    tnow(i)%v = tnow0(i)
    do j=1,kdim
      if (i==j) then
        tnow(i)%d(j) = 1.0D0
      else
        tnow(i)%d(j) = 0.0D0
      end if
    end do
  end do
  call box_timestep(gamma_t,tStar,nullforce,uvel,tnow,told,tnew)
  do i=1,kdim
    do j=1,kdim
      write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",j,")=",tnew(j)%d(i)
    end do
  end do
  close(2)

end program driver

