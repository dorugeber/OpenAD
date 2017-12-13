c$openad XXX Template ad_template.f
       subroutine foo() 
          implicit double precision (s,t)
          common /cb/ s,t
          t=sin(s)
       end subroutine

c$openad XXX Template ad_template.f
       subroutine head(x,y) 
          implicit double precision (s,t)
          common /cb/ s,t
          double precision, dimension(1), intent(inout) :: x
          double precision, dimension(1), intent(inout) :: y
c$openad INDEPENDENT(x)
          s=x(1)
          call foo()
          y(1)=t
c$openad DEPENDENT(y)
       end subroutine
