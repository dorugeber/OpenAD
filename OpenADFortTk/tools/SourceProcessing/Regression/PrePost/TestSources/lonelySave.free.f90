subroutine foo(i)
  SAVE j 
  if (i==0) then 
   j=i+1
  else 
   j=j+1
   i=i+j
  end if 
end 

program p
  i=0
  call foo(i)
  i=1
  call foo(i)
  print *,i
end 
  
