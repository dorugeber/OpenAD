module OAD_intrinsics
end module
subroutine foo
use OAD_intrinsics
common /c/ x(3)
real :: x
print *,x
end

subroutine bar
use OAD_intrinsics
common /c/ x
real :: x(3)
print *,x
end

program p
use OAD_intrinsics
common /c/ x(3)
real :: x
x=(/.1,.2,.3/)
call foo
call bar
end
