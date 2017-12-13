program driver

  use OAD_active
  use OAD_rev
  use OAD_tape

  implicit none 

  external head

  ! inputs
  type(active) x1, x2
  type(active) x_ph1, x_ph2
  ! output
  type(active) y
  type(active) y_ph

  double precision, dimension(2) :: x0
  double precision, dimension(1,2) :: res_adj
  double precision :: h
  integer :: n, m
  integer i,j,k

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)

  do i=1,n   
     x0(i)=sqrt(1.*i)
  end do

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  our_rev_mode%arg_store=.FALSE.
  our_rev_mode%arg_restore=.FALSE.
  our_rev_mode%res_store=.FALSE.
  our_rev_mode%res_restore=.FALSE.
  our_rev_mode%plain=.TRUE.
  our_rev_mode%tape=.FALSE.
  our_rev_mode%adjoint=.FALSE.
  x1%v=x0(1)
  x2%v=x0(2)

  call head (x1, x2, y)

  do i=1,n   
     x_ph1%v=x0(1)
     x_ph2%v=x0(2)

     if (i==1) x_ph1%v=x_ph1%v+h
     if (i==2) x_ph2%v=x_ph2%v+h
     call head (x_ph1, x_ph2, y_ph)
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",i,")=",(y_ph%v-y%v)/h
  end do
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  call tape_init()
  do i=1,m
     y%v=0.0
     if (i==1) y%d=1.0

     x1%v=x0(1)
     x2%v=x0(2)

     x1%d=0.
     x2%d=0.
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.TRUE.
     our_rev_mode%adjoint=.FALSE.
     call head (x1, x2, y)
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.FALSE.
     our_rev_mode%adjoint=.TRUE.
     call head (x1, x2, y)
     do k=1,n
        if (k==1)  res_adj(i,k)=x1%d
        if (k==2)  res_adj(i,k)=x2%d
     end do
  end do
  do k=1,n
     do i=1,m   
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",k,")=",res_adj(i,k)
     end do
  end do
  close(2)

end program driver
