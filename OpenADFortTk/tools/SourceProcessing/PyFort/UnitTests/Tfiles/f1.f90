subroutine foo(x)
  x = x + &
!
! embedded continuation lines
!
  &5 + 2 * x&
!
! more embedded lines
!     
  & + si&
  &n(x+2.0)
  x = 5.0
  x&
  & = &
  & 13.2
end subroutine foo
