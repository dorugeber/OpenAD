      module m 
      public :: foo
         interface foo
           module procedure foo_i
         end interface
      contains
          subroutine foo_i(x)
           double precision :: x
           x=x*2
         end subroutine
      end module 

c$openad XXX Template ad_template.f
      subroutine head(x,y) 
      use m
      double precision, dimension(1), intent(inout) :: x
      double precision, dimension(1), intent(inout) :: y
c$openad INDEPENDENT(x)
          call foo(x(1))
          y(1)=x(1)
c$openad DEPENDENT(y)
      end subroutine
