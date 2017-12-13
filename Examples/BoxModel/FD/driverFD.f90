program boxmodel_fd_driver

  use all_globals_mod
  implicit none

  integer, parameter :: kdim=3
  integer i,j
  double precision :: h=.00001
  double precision tnew_o(kdim), snew_o(kdim)
  double precision jac(2*kdim,2*kdim)

  external box_model_body

  write(*,*) "divided differences:"
  do i=1,2*kdim
     call box_ini_params
     call box_ini_fields
     call box_model_body
     do j=1,kdim
        tnew_o(j)=tnew(j)
     end do
     do j=1,kdim
        snew_o(j)=snew(j)
     end do
     call box_ini_params
     call box_ini_fields
     xx(i)=xx(i)+h
     call box_model_body
     do j=1,kdim
        jac(j,i)=(tnew(j)-tnew_o(j))/h
        jac(j+kdim,i)=(snew(j)-snew_o(j))/h
     end do
  end do
  do i=1,2*kdim
     do j=1,2*kdim
        write(*,'(2(A,I1),A,EN26.16E3)') "F(",i,",",j,")=",jac(i,j)
     end do
  end do

end program boxmodel_fd_driver
