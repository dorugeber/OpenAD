subroutine driver_h(x,h)

  use active_module
  use OpenAD_rev
  use OpenAD_tape

  implicit none 

  double precision, dimension(2) :: x 
  double precision, dimension(2,2) :: h 

  type(active) , dimension(2) :: active_x
  type(active) , dimension(2) :: active_g
  integer i,j

  do i=1,2
     x(i)=i*1.0
  end do
  ! reverse for the gradient
  call tape_init()
  do i=1,2
     do j=1,2
        ! initialize values (driver_g inits the gradient)
        active_x(j)%d=0.0
        active_x(j)%v=x(j)
        if ( i.eq.j) then
           active_g(j)%d=1.0 
        else
           active_g(j)%d=0.0 
        end if
     end do
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.TRUE.
     our_rev_mode%adjoint=.FALSE.
     call driver_g(active_x,active_g)
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.FALSE.
     our_rev_mode%adjoint=.TRUE.
     call driver_g(active_x,active_g)
     do j=1,2
        h(i,j)=active_x(j)%d
     end do
  end do

end subroutine driver_h
