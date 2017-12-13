program boxmodel_driver

  use all_globals_mod
  implicit none

  integer, parameter :: kdim=3
  integer i,j
  external box_model_body

     call box_ini_params
     call box_ini_fields
     call box_model_body
     do j=1,2*kdim
        write(*,'(2(A,I1,A,EN26.16E3))')" tnew(",j,")=",tnew(j),&
                                        " snew(",j,")=",snew(j)
     end do

end program boxmodel_driver
