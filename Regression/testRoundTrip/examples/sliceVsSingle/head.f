      subroutine foo(x)
        double precision :: x
        x=x+1.0
      end subroutine
      
      subroutine bar(x)
        double precision :: x(1)
        x(1)=x(1)+1.0
      end subroutine 
      
      subroutine head(x,y)
        interface 
           subroutine foo(x)
             double precision :: x
           end subroutine foo
        end interface
        interface 
           subroutine bar(x)
             double precision :: x(1)
           end subroutine bar
        end interface
        double precision, dimension(2) :: x,y
c$openad INDEPENDENT(x)
        call foo(x(1))
        y=x
        call bar(y(2:2))
c$openad DEPENDENT(y)
      end subroutine
