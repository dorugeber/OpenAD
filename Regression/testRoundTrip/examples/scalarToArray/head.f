c$openad XXX Template ad_template.f
        subroutine head(x,y)
          implicit none
          double precision,dimension(1) :: x
          double precision,dimension(2) :: y
c$openad INDEPENDENT(x)
          y=x(1)
c$openad DEPENDENT(y)
        end subroutine head
