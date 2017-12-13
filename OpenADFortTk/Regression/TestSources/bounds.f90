program p
  double precision :: x(3)
  integer :: i, l(1)
  do i=lbound(x,1), ubound(x,1)
    x(i)=i*0.5D0
  end do
  l=lbound(x)
  print *,x, l
end program
