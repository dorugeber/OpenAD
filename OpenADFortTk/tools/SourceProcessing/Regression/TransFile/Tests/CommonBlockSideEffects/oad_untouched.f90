      subroutine faa(x,y)
      common /cb/ g
      double precision g,x,y
      g=g*2.0
      y=x*g
      end subroutine
