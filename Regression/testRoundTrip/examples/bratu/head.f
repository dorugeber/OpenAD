c$openad XXX Template ad_template.f
      subroutine head(x,F)
          double precision :: x(9)
          double precision :: F(7)
          integer i
          double precision h 

c$openad INDEPENDENT(x)      
   
          h = 2.0D0/8.D0
          F(1) = -2*x(1)+h*h*x(7+1)/12.0D0*(1+10*exp(x(1)/(1.0D0+x(7+2)
     &*x(1))))
          F(2) = x(1)+h*h*x(7+1)/12.0D0*exp(x(1)/(1.0D0+x(7+2)*x(1)))
          do i=2,6
             F(i-1) = F(i-1)+x(i)+h*h*x(7+1)/12.0D0*exp(x(i)/(1.0D0+x(7
     &+2)*x(i)))
             F(i) = F(i)-2*x(i)+h*h*x(7+1)/1.2D0*exp(x(i)/(1.0D0+x(7+2)
     &*x(i)))
             F(i+1) = x(i)+h*h*x(7+1)/12.0D0*exp(x(i)/(1.0D0+x(7+2)
     &*x(i)))
          end do
          F(7-1) = F(7-1)+x(7)+h*h*x(7+1)/12.0D0*exp(x(7)/    
     &      (1.0D0+x(7+2)*x(7)))
          F(7) = F(7)-2*x(7)
          F(7) = F(7)+h*h*x(7+1)/12.0D0*(1+10*exp(x(7)/        
     &      (1.0D0+x(7+2)*x(7))))

c$openad DEPENDENT(F)

      end subroutine head

