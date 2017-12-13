module m 
  integer i
end module 

program p 
  use m, only: 
  integer i ! leads to a name clash if only is dropped
  i=1
  print*, 'OK'
end program
