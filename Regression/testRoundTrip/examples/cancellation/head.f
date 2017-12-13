c$openad XXX Template ad_template.f
      SUBROUTINE head (x1, x2, y) 
      IMPLICIT none               
      double precision,intent(in):: x1, x2
      double precision,intent(out):: y
      double precision:: a, b, c, d, e, f
c$openad INDEPENDENT(x1)
c$openad INDEPENDENT(x2)
      a = (1.0d0/(x1/x2))
      b = a*x2
      c = x2
c---------------------------------------
      c = sin(c)
      d = a
      f = a
c---------------------------------------
      f = 0.5d0*(f - a)
      e = 0.5d0*(d - a)
      y = c*e + f*b
c$openad DEPENDENT(y)
      END SUBROUTINE head
