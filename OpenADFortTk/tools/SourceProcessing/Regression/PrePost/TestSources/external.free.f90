integer function foo ()
   integer, save :: fooi=1
   fooi=fooi+1
   foo=fooi
end function


module m 

real :: r

contains 

  subroutine bar()

    external foo
    integer foo
    print *, r, foo() 

  end subroutine 
end module 

subroutine  head ()
  use m 
external foo
integer foo
  r=3.0
  call bar()
  print *,foo()
end subroutine 

program p 

call head()
end program

