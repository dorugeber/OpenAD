  subroutine head(x,y)
  interface
    subroutine foo(x,y)
      double precision x
      double precision y
    end subroutine
    function foo2(a)
      implicit integer (a,f)
    end
  end interface

  integer,dimension(4) :: x
  complex :: y

  type bar
     real(some) x,y(10)
     complex,dimension(6) other
  end type bar

  type(bar) :: v

  call foo(3.4,2.7)
  x(1) = 1 + int(sin(2.3))
  x(2) = int(real(y))

  end subroutine head
