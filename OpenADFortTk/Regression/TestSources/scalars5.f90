!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with scalar operations
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine d_head(x, d_x, y, d_y) 
  double precision, intent(in) :: x, d_x
  double precision, intent(inout) :: y, d_y
  double precision t1, t2, xb_aux_1, xb_aux_2 
  double precision xb_aux_3, xb_aux_4, xb_aux_5
  integer i

  i=1
  xb_aux_1=sin(x)
  xb_aux_2=cos(x)
  t1=xb_aux_1*y
  t2=t1*x+i
  y=t1+t2

  xb_aux_3=x+1
  xb_aux_4=t1+xb_aux_2*y*xb_aux_3
  xb_aux_5=xb_aux_1*xb_aux_3

  d_y = d_x*xb_aux_4+d_y*xb_aux_5

end subroutine


program deriv

  double precision :: x, d_x, y, d_y
  x = 45
  d_x = 9;
  y = 60
  d_y = 13;

  call d_head(x, d_x, y, d_y)
  write(*,*) y
  write(*,*) d_y

end program

