  subroutine head(x,y)
  interface
    subroutine foo(x,y)
      double precision x
      double precision y
    end subroutine
  end interface
  integer,dimension(4) :: x
  complex :: y

  call foo(3.4,2.7)
  x(1) = 1 + int(sin(2.3))
  x(2) = int(real(y))

  end subroutine head
