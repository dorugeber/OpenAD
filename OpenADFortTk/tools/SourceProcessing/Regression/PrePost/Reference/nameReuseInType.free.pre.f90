module OAD_intrinsics
end module
module m1
  integer :: mi
end module

module m2
 use m1
 type t2
   integer::mi
   integer::mi2
 end type
end module

program p
 use OAD_intrinsics
 use m2
 implicit none
 type(t2) :: at2
 at2%mi=3
 at2%mi2=4
 mi=1
 print *,at2
end program
 
