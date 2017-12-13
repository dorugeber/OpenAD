module OAD_intrinsics
end module
subroutine foo(i)
  use OAD_active
  use OAD_intrinsics
  SAVE j
  if (i==0) then
   j = i+1
  else
   j = j+1
   i = i+j
  end if
end

program p
  use OAD_active
  use OAD_intrinsics
  i=0
  call foo(i)
  i=1
  call foo(i)
  print *,i
end
  
