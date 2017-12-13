!$openad xxx p1end
!$openad xxx file_start p1a p1aend
module a
  integer ai
end module 
!$openad xxx FILE_start p2 p2end 
!$openad xxx p2a p2aend
module b 
  integer bi 
end module 
!$openad xxx file_start  p3a p3aend
!$openad xxx p3 p3end
program p 
 use a
 use b 
 print *, 'OK'
end program
