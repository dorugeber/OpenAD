module OAD_intrinsics
end module
program p
 use OAD_active
 use OAD_intrinsics
 integer i
 i=4
 goto (10,20,30), i
 go to (10,20,30) i-1
 10            print *,'10'
 20            print *,'20'
 goto 50
 30            print *,'30'
 50            continue
end

