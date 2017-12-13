      double precision function bar(x)
        double precision,intent(in) :: x
        bar = max(x,4.0D0)
      end function bar

c$openad XXX Template ad_template.f
      subroutine head(x,y) 
        implicit none
        double precision, dimension(1), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
        double precision :: t
        double precision :: bar
c$openad INDEPENDENT(x)
        y(1) = bar(x(1))*2.0D0
        t = bar(y(1))
c$openad DEPENDENT(y)
      end subroutine head

