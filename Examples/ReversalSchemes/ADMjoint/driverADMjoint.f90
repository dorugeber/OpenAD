program boxmodel_adm_driver

  use OAD_active
  use OAD_rev
  use OAD_tape
  use RevStats
  use all_globals_mod
  implicit none

  integer, parameter :: kdim=3
  integer i,j
  double precision jac(2*kdim,2*kdim)


  external box_model_body

  write(*,*) "adjoint model, joint mode"
  call oad_tape_init()
  do i=1,2*kdim
     call revStatsInit()
     do j=1,2*kdim
        call zero_deriv(xx(j))
     end do
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.TRUE.
     our_rev_mode%tape=.FALSE.
     our_rev_mode%adjoint=.FALSE.
     call box_ini_params
     call box_ini_fields
     do j=1,kdim
        call zero_deriv(tnew(j))
        call zero_deriv(snew(j))
     enddo
     if (i>kdim) then
        do j=1,kdim
           if (j==i-kdim) then
              snew(j)%d=1
           end if
        end do
     else
        do j=1,kdim
           if (j==i) then
              tnew(j)%d=1
           end if
        end do
     end if
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.TRUE.
     our_rev_mode%adjoint=.TRUE.
     call box_model_body
     do j=1,2*kdim
        write(*,'(2(A,I1),A,EN26.16E3)') "F(",i,",",j,")=",xx(j)%d
     end do
  end do
  call revStatsDump()

end program boxmodel_adm_driver
