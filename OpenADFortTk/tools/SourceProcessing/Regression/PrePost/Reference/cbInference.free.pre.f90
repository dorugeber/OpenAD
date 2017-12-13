module OAD_intrinsics
end module
subroutine foo(x)
use OAD_intrinsics
IMPLICIT DOUBLE PRECISION (A-H,O-Z)
integer NLAYR2
COMMON /CMPMR4/ CHFCOF(2,3), NLAYR2(3)
dimension x(3)
x=chfcof(1,:)
end subroutine

program p
use OAD_intrinsics
IMPLICIT DOUBLE PRECISION (A-H,O-Z)
COMMON /CMPMR4/ CHFCOF(2,3), NLAYR2(3)
dimension x(3)
chfcof(1,:)=(/1.1,1.2,1.3/)
call foo(x)
print *,x
end program
