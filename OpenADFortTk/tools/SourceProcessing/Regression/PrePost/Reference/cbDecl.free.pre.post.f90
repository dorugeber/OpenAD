module OAD_intrinsics
end module
subroutine foo
use OAD_active
use OAD_intrinsics
common /c/ x(3)
real :: x
print *,x
end

program p
use OAD_active
use OAD_intrinsics
common /c/ x(3)
real :: x
x=(/.1,.2,.3/)
call foo()
end
