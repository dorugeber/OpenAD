      subroutine foo(x,y)

      character*22,dimension(2) :: x*3
      character(27)   :: y(2,4)*12

      call bar(x(k),y)

      end
