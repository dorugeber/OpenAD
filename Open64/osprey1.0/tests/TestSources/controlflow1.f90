subroutine head(x,y)
  integer, dimension(1), intent(in) :: x
  integer, dimension(1), intent(out) :: y
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
  integer, dimension(1) :: x,y
  x(1)=2
  call head(x,y)
  if (y(1) .eq. x(1)**4 ) then 
    print *, 'OK'
  else 
    print *,'failed'
  endif
end program
