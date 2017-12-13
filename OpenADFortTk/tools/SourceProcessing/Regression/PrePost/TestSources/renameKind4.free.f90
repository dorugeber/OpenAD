module m1
  integer,parameter :: x=5
end module 

module m2
  use m1, only: z=>x
  contains 

    function foo()
      real :: foo
      foo=2.0D0
    end function 
end module 

program p
  use m2
  use m1, only: y=>x
  real, dimension(y) :: x
  x(1)=foo()
  print *,x(1)
end program
  
