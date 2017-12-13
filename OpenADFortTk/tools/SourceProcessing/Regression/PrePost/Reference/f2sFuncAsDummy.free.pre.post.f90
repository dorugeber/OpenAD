module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics
contains
! the function we will pass
function foo(xfoo)
  real:: xfoo,foo
  foo = 2*xfoo
end function
! the function we will pass
subroutine oad_s_foo(xfoo,foo)
  real :: xfoo
  real,intent(out) :: foo
  foo = 2*xfoo
end subroutine oad_s_foo
end module

! the function that takes another function as dummy
function bar(f,xbar)
  use OAD_active
  use OAD_intrinsics
  real::xbar,bar
  interface
    function f(xf)
      use OAD_active
      real:: xf,f
    end function
  end interface
  bar = f(xbar)
end function bar

! the function that takes another function as dummy
subroutine oad_s_bar(oad_s_f,xbar,bar)
  use OAD_active
  use OAD_intrinsics
  real :: xbar
  interface
    function f(xf)
      use OAD_active
      real:: xf,f
    end function
    subroutine oad_s_f(xf,f)
      use OAD_active
      real,intent(out) :: f
      real :: xf
    end subroutine
  end interface
  real :: oad_ctmp0
  real,intent(out) :: bar
  call oad_s_f(xbar,oad_ctmp0)
  bar = oad_ctmp0
end subroutine oad_s_bar

program main
  use OAD_active
  use OAD_intrinsics
  use m
  real:: x,y
  external oad_s_bar
  real :: oad_ctmp0
  x=3.5
  call oad_s_bar(oad_s_foo,x,oad_ctmp0)
  y = oad_ctmp0
  print *,y
end program
