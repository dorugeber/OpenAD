      subroutine bar()
      call foo()
      call foo1()
      end subroutine
      subroutine foo()
      common /cb/ g
      double precision g
      g=g*2.0
      end subroutine
      subroutine foo1()
      common /cb/ g
      double precision g
      g=g*3.0
      end subroutine
      subroutine head(x,y) 
      common /cb/ g
      double precision g
      double precision, dimension(1), intent(inout) :: x
      double precision, dimension(1), intent(inout) :: y
c$openad INDEPENDENT(x)
          g=x(1)
          call bar()
          y(1)=g
c$openad DEPENDENT(y)
       end subroutine
