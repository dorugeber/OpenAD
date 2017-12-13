module OAD_intrinsics
end module
module m1
  use OAD_active
  integer,parameter :: x=5
  integer :: blah=3
end module

module m2
use OAD_active
use OAD_intrinsics
  interface oad_s_foo_bar
     module procedure oad_s_foo,oad_s_foo1
  end interface
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

    subroutine oad_s_foo(x,foo)
      use m1, only: z=>x, blah
      real,dimension(blah) :: x
      real,intent(out) :: foo
      foo=2.0D0
    end subroutine oad_s_foo
    subroutine oad_s_foo1(x,foo1)
      use m1, only: z=>x, blah
      double precision,dimension(blah) :: x
      real,intent(out) :: foo1
      foo1=2.0D0
    end subroutine oad_s_foo1
end module

program p
  use OAD_active
  use OAD_intrinsics
  use m2
  use m1, only: y=>x
  real, dimension(y) :: x
  real, dimension(3) :: t
  real :: oad_ctmp0
  call oad_s_foo_bar(t,oad_ctmp0)
  x(1) = oad_ctmp0
  print *,x(1)
end program
  
