module m1
  integer , parameter :: double=selected_real_kind(14)
end module

module m2
  contains

    function foo()
      use m1,  dp=>double
      real(dp) :: foo
      foo=2.0D0
    end function
end module

program p
  use m1, dpp=>double
  use m2
  real(dpp)::x
  x=foo()
  print *,x
end program

