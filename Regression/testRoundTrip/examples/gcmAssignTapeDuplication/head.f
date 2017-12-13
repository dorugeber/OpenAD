c$openad XXX Template ad_template.f
        subroutine head(x,y) 
          implicit none
          double precision, dimension(12), intent(in) :: x
          double precision, dimension(1), intent(out) :: y
c$openad INDEPENDENT(x)
          y(1) = (x(3)*((x(4)* x(2) + x(1)*(x(5) +x(6) + 
     >           x(7) + x(8))) /(x(2) + x(1) *(x(9) + 
     >           x(10) + x(11) + x(12)))))
c$openad DEPENDENT(y)
        end subroutine head

