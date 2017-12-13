module OAD_intrinsics
end module
program p
use OAD_active
use OAD_intrinsics
character b
B='3'
IF (LGE(B,'0').AND.LLE(B,'9')) THEN
print *,B
end if
end
