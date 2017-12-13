module m1
  integer , parameter :: double=selected_real_kind(14)
end module 

module m2
  use m1,  dp=>double
  contains 

    function foo()
      real(dp) :: foo
      foo=2.0D0
    end function 
end module 

program p
  use m2
  use m1, dpp=>double
  real(dp) ::y
  real(dpp)::x
  x=foo()
  print *,x
end program
  
