c$openad XXX Template ad_template.f
      subroutine head(x,y) 
      double precision, dimension(2), intent(inout) :: x
      double precision, dimension(1), intent(inout) :: y
      real(8) :: p1, p2
c$openad INDEPENDENT(x)
          p1=2.4D0
          p2=-30.0D0
          y(1)=min(x(1),x(2))*max(p1,p2)
c$openad DEPENDENT(y)
      end subroutine
