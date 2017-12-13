subroutine head(x,y)
  double precision, dimension(1), intent(in) :: x
  double precision, dimension(1), intent(out) :: y
  integer i


  y(1)=x(1) 
  if (y(1)>0.) then
     do i=1,3
        y(1)=y(1)*x(1)
     end do
  else
     y(1)=y(1)/x(1)
  end if


end subroutine head

program controlFlow1
  double precision, dimension(1) :: x,y
  x(1)=2.3D0
  call head(x,y)
  print *,y(1)
end program
