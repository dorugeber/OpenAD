program allocatedarray
  double precision, allocatable, dimension(:) :: x
  double precision :: y
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  allocate(x(1))
  y=x(1)*2
  if (allocated(x)) then 
    print *, 'OK' 
  end if
end program
