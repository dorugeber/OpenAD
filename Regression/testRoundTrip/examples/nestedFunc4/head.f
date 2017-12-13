      module m 
      contains 
          subroutine foo(x,y)
            implicit none
            double precision, dimension(2) :: x
            double precision, dimension(1) :: y
            integer i 
            y(1)=x(1)*x(2)
            i=2
            call bar()
            contains 
               subroutine bar()
                 implicit none
                 y(1)=y(1)*sin(x(1))*cos(x(i))
               end subroutine
            end subroutine 
      end module 

c$openad XXX Template ad_template.f
      subroutine head(x,y) 
        use m 
        implicit none
        double precision, dimension(2) :: x
        double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
          call foo(x,y)
c$openad DEPENDENT(y)
      end subroutine
