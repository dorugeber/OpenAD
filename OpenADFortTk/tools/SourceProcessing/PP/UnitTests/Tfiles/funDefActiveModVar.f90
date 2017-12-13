module active_var
  type(oadactive) :: foo
end module

real function bar(x)
  use OAD_intrinsics
  use active_var
  real :: x
  bar = x*2*foo
  return
end function
subroutine oad_s_bar(x,bar)
  use OAD_intrinsics
  use active_var
  real :: x
  real,intent(out) :: bar
  bar = x*2*__value__(foo)
  return
end subroutine
