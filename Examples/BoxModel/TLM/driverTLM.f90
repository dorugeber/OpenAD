program boxmodel_tl_driver

  use OAD_active
  use all_globals_mod
  implicit none

  integer, parameter :: kdim=3
  integer i,j
  double precision jac(2*kdim,2*kdim)

  external box_model_body

  write(*,*) "scalar tangent linear mode"
  do i=1,2*kdim
     call box_ini_params
     call box_ini_fields
     do j=1,2*kdim
        call zero_deriv(xx(j))
     enddo
     xx(i)%d=1.D0
     call box_model_body
     do j=1,kdim
        jac(j,i)=tnew(j)%d
        jac(j+kdim,i)=snew(j)%d
     end do
  end do
  do i=1,2*kdim
     do j=1,2*kdim
        write(*,'(2(A,I1),A,EN26.16E3)') "F(",i,",",j,")=",jac(i,j)
     end do
  end do

end program boxmodel_tl_driver
