      subroutine foo(x,y) 
        double precision, dimension(2),intent(in) :: x
        double precision, dimension(1),intent(out) :: y
        double precision :: t
        t=x(1)*x(2)
        y(1)=t
      end subroutine

      subroutine head(x,y) 
	interface 
          subroutine foo(x,y) 
            double precision, dimension(2),intent(in) :: x
            double precision, dimension(1),intent(out) :: y
          end subroutine
        end interface
        double precision, dimension(2) :: x
        double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
        call foo(x,y)
c$openad DEPENDENT(y)
       end subroutine
