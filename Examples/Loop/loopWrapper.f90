!$openad XXX Template ad_revolve_template.f
subroutine loopWrapper(x,n) 
  double precision :: x
  integer :: n
!$openad INDEPENDENT(x)
  do i=1,n
    call loopBody(x)
  end do
!$openad DEPENDENT(x)
end subroutine
