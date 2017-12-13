      subroutine faa(x,y)
      use OAD_active
      common /cb/ g
      double precision :: x,y
      type(active) :: g
      g%v = g%v*2.0
      y = x*g%v
      end subroutine
