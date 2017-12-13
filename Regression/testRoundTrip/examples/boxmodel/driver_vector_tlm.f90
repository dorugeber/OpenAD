program boxmodel_tl_driver

  use OAD_active
  use all_globals_mod
  implicit none

  integer,parameter :: kdim=3
  integer :: i,j,n,m
  double precision :: h
  double precision tnew_o(kdim), snew_o(kdim)
  double precision jac(2*kdim,2*kdim)


  external box_model_body

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n,m,h
  close(2)

  !
  ! DD code
  !
  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  do i=1,2*kdim
     call box_ini_params
     call box_ini_fields
     call box_model_body
     do j=1,kdim
        tnew_o(j)=tnew(j)%v
     end do
     do j=1,kdim
        snew_o(j)=snew(j)%v
     end do
     call box_ini_params
     call box_ini_fields
     xx(i)%v=xx(i)%v+h
     call box_model_body
     do j=1,kdim
        jac(j,i)=(tnew(j)%v-tnew_o(j))/h
        jac(j+kdim,i)=(snew(j)%v-snew_o(j))/h
     end do
  end do
  do i=1,2*kdim
     do j=1,2*kdim
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",j,")=",jac(i,j)
     end do
  end do
  close(2)

  !
  ! ACTS code
  !
  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  call box_ini_params
  call box_ini_fields
  do i=1,2*kdim
     call zero_deriv(xx(i))
     do j=1,2*kdim
        if (i==j) xx(i)%d(j) = 1.D0
     enddo
  end do
  call box_model_body
  do i=1,2*kdim
     do j=1,kdim
        jac(j,i) = tnew(j)%d(i)
        jac(j+kdim,i) = snew(j)%d(i)
     end do
  end do

  do i=1,2*kdim
     do j=1,2*kdim
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",j,")=",jac(i,j)
     end do
  end do
  close(2)

end program boxmodel_tl_driver

