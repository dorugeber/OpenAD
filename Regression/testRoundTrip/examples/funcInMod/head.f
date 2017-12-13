      module m 
      public :: foo, mx
         interface foo
           module procedure foo_i
           module procedure foo_ii
         end interface
         double precision :: mx
      contains
         double precision function foo_i(x)
           double precision :: x
           mx=mx+x
           foo_i=mx
         end function
         integer function foo_ii(x)
           integer :: x
           foo_ii=x+1
         end function
      end module 

c$openad XXX Template ad_template.f
      subroutine head(x,y) 
      use m
      double precision, dimension(1), intent(inout) :: x
      double precision, dimension(1), intent(inout) :: y
c$openad INDEPENDENT(x)
          mx=x(1)
          y(1)=foo(foo(x(1)))
          i=foo(2)
c$openad DEPENDENT(y)
      end subroutine
