subroutine myloop(x)
  double precision, intent(inout) :: x
  do while (x > 0)
     x=x-.3
     if (x < 0) return
  end do
end subroutine

program controlflow
  double precision :: x
  x = 2
  call myloop(x)
  print *,x
end program
