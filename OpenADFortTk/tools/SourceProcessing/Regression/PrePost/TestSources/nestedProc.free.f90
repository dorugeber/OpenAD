subroutine s1()
  real :: x
  contains
  real function foo(x)
    integer:: x
    foo=x
  end function
end subroutine

subroutine s2()
  real :: x
  contains
  real function foo(x)
   integer :: x
   foo=x
  end function
end subroutine

program p 
  call s1
  call s2
end program
