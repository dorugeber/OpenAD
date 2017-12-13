subroutine myloop(x)
  double precision, intent(inout) :: x
d1: do while (x > 0)
     x=x-.3
     do j=1,3
       if (x < 0) exit d1
       x=x+.01D0
     end do
  end do d1
end subroutine

program controlflow
  double precision :: x
  x = 2
  call myloop(x)
  print *,x
end program
