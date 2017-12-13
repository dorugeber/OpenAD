c$openad xxx Template ad_template.f
      subroutine head(x,y)
      double precision, dimension(1) :: x,y
c$openad INDEPENDENT(x)
         goto 1
    3 x(1)=x(1)*2
         goto 4 
    2 x(1)=x(1)*3 
         goto 3
    1 x(1)=x(1)*x(1)
         goto 2
    4 y(1)=x(1)        
c$openad DEPENDENT(y)
      end subroutine
