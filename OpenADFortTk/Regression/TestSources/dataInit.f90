subroutine foo(x,y)
  real ::x,y
  real :: local, other
  data local /1.0/
  other=1.0
  x=local
  y=other
  local=local*2
  other=other*2
end subroutine

program p
 real :: x,y
 integer :: errors
 errors=0
 call foo(x,y)
 if (x.ne.1.0 .and. x.ne.1.0) then 
   print *,x,y
   errors=errors+1
 end if
 call foo(x,y)
 if (x.ne.2.0 .and. x.ne.1.0) then
   print *,x,y
   errors=errors+1
 end if
 if (errors==0) then 
   print *,'OK'
 end if
end program

