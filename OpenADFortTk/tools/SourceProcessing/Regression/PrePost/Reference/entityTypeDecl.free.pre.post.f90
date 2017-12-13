module OAD_intrinsics
end module
program p
use OAD_active
use OAD_intrinsics
implicit none
dimension a(3)
real a
data a /.1,.2,.3/
print *,a
end
