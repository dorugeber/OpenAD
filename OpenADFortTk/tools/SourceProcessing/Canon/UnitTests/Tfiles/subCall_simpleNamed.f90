program subCall_simpleNamed
  real :: v

  v = 1.4
  call foo(v,b=v)
  print *,v

  contains
    subroutine foo(a,b)
      real :: a,b
      a = b
    end subroutine foo
end program subCall_simpleNamed

