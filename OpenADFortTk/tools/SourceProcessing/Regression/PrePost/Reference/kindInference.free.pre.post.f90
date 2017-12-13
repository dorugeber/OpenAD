module OAD_intrinsics
end module
module m
 use OAD_active

 integer , parameter :: dp=kind(1.0D0)

end module

module m1
 use OAD_active
 use m
 integer, parameter :: rk=dp
end module

subroutine foo(x)
 use OAD_active
 use OAD_intrinsics
 double precision :: x
 print *,x
end subroutine

program p
 use OAD_active
 use OAD_intrinsics
 use m
 use m1
 real(dp) :: x1
 real (kind=rk) :: x2
  double precision :: oad_ctmp0
 x1=1.0D0
 x2=2.0D0
 oad_ctmp0 = x1+x2
 call foo(oad_ctmp0)
! determine the kind of the temporary
end program
