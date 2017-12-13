module OAD_intrinsics
end module
program p
 use OAD_intrinsics
 integer i
 i=2
 if (i<2) then
   goto 1
 end if
 go to 2
 1      print *,'1'
 goto 3
 2      print *,'2'
 3      continue
end

