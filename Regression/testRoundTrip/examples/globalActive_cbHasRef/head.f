      subroutine bar()
      call foo()
      call foo1()
      end subroutine
      subroutine foo()
      common /cb/ g
      double precision g
      common /cb1/ g1
      double precision g1
      g1=g*2.0
      end subroutine
      subroutine foo1()
      common /cb/ g
      double precision g 
      common /cb1/ g1
      double precision g1
      g=g1*3.0
      end subroutine
      subroutine baz()
      common /cb/ g
      double precision g
      common /cbxy/ x,y
      double precision x(1),y(1)
          g=x(1)
          call bar()
          y(1)=g
       end subroutine
      subroutine head() 
      common /cbxy/ x,y
      double precision x(1),y(1)
c$openad INDEPENDENT(x)
          x=x ! provide some reference
          y=y ! provide some reference
          call baz()
c$openad DEPENDENT(y)
       end subroutine
