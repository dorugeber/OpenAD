module active_var
  use OAD_active
  type(active) :: foo
end module

real function bar(x)
  use OAD_active
  use OAD_intrinsics
  use active_var
  real :: x
  bar = x*2*foo%v
  return
end function
subroutine oad_s_bar(x,bar)
  use OAD_active
  use OAD_intrinsics
  use active_var
  real :: x
  real,intent(out) :: bar
  bar = x*2*foo%v
  return
end subroutine
