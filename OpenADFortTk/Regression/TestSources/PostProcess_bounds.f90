program bounds
  double precision, target :: x(3)
  double precision, pointer :: p(:)
  integer :: i, l(1)
  p=>x
!$openad INDEPENDENT(X)
  do i=lbound(p,1), ubound(p,1)
    p(i)=x(i)
    x(i)=i*0.5D0+p(i)
  end do
!$openad DEPENDENT(X)
  print *,x
end program
