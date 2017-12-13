program implicitArrayBound
  double precision, allocatable, dimension(:) :: a
  double precision, dimension(3) :: b

  b(1) = 1.0D0
  b(2) = 2.0D0
  allocate(a(3))
  a(:2) = b(1:2)
  print *,'a(1) = ', a(1), 'a(2) = ',a(2)
end program implicitArrayBound
