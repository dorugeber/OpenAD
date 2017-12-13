module m1
  integer,parameter :: x=5
  integer :: blah=3
end module 

module m2
  interface foo_bar
     module procedure foo, foo1
  end interface
  contains 

    function foo(x)
      use m1, only: z=>x, blah
      real :: foo
      real,dimension(blah) :: x
      foo=2.0D0
    end function 
    function foo1(x)
      use m1, only: z=>x, blah
      real :: foo1
      double precision,dimension(blah) :: x
      foo1=2.0D0
    end function 
end module 

program p
  use m2
  use m1, only: y=>x
  real, dimension(y) :: x
  real, dimension(3) :: t
  x(1)=foo_bar(t)
  print *,x(1)
end program
  
