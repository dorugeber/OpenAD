subroutine foo(i)
integer i
  i=i+1
end subroutine

subroutine bar(k,s)
integer k
call s(k)
end subroutine 

program srAsParameter
external foo
integer i
i=3
call bar(i,foo)
if (i== 4) then 
print *,'OK'
else
print *,'failed'
end if
end program
