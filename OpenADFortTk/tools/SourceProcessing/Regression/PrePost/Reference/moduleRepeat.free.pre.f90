module OAD_intrinsics
end module
module m
 public
 real :: x(3)
end module

subroutine foo
use OAD_intrinsics
use m
print *,x
end

subroutine bar
use OAD_intrinsics
use m
print *,x
end

program p
use OAD_intrinsics
use m
x=(/.1,.2,.3/)
call foo
call bar
end
